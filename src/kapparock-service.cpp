//====================================================================================
//			The MIT License (MIT)
//
//			Copyright (c) 2011 Kapparock LLC
//
//			Permission is hereby granted, free of charge, to any person obtaining a copy
//			of this software and associated documentation files (the "Software"), to deal
//			in the Software without restriction, including without limitation the rights
//			to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//			copies of the Software, and to permit persons to whom the Software is
//			furnished to do so, subject to the following conditions:
//
//			The above copyright notice and this permission notice shall be included in
//			all copies or substantial portions of the Software.
//
//			THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//			IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//			FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//			AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//			LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//			OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//			THE SOFTWARE.
//====================================================================================

#include "notification.h"
#include "kjson.h"
#include "restful.h"
#include "hal.h"
#include "kapparock-service.h"

using namespace kapi;

void init()
{
	using Context = ApplicationInterface::Context;

	notify::handler("Application", "zigbee_module/assoc_count", [](Context ctx)
	{
		JSON rsp(JSONType::JSON_OBJECT);
		int count = getAssocCount();
		rsp["assoc_count"] = count;
		ctx.response(rsp.stringify());
	});

	notify::handler("Application", "zigbee_module/assoc_find_device", [](Context ctx)
	{
		JSON arg(ctx.parameter().c_str());
		HAL_STRUCTURE::associated_devices_t dev;
		JSON rsp(JSONType::JSON_OBJECT);
		uint8_t number = arg["number"].toInteger();

		int status = getAssocInfo(&dev, number);
		rsp["status"] = status;
		rsp["device"] = { kapi::JSONType::JSON_OBJECT };
		JSON& devJson = rsp["device"];
		devJson["nwkAddr"] =  IntToHexStr(dev.shortAddr);
		devJson["ieeeAddr"] = IntToHexStr(getAssocExtAddr(dev.shortAddr));
		devJson["age"] =  dev.age;
		devJson["assocCnt"] = dev.assocCnt;
		devJson["devStatus"] = IntToHexStr(dev.devStatus);
		devJson["nodeRelation"] = dev.nodeRelation;
		ctx.response(rsp.stringify());
	});

	return;
}
