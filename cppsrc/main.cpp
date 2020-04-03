#include "stdafx.h"

World world(480, 270);

Napi::Object stepWrapped(const Napi::CallbackInfo& info)
{
	Napi::Env env = info.Env();
	return world.step(env);
}

Napi::Number addWrapped(const Napi::CallbackInfo& info)
{
	Napi::Env env = info.Env();
	auto number = Napi::Number::New(env, 0);
	if (info[0].IsString()) world.addPlayer(info[0].As<Napi::String>().Utf8Value());
	return number;
}

Napi::Number accelWrapped(const Napi::CallbackInfo& info)
{
	Napi::Env env = info.Env();
	auto number = Napi::Number::New(env, 0);
	if (info[0].IsNumber() && info[1].IsNumber() && info[2].IsString()) world.accelPlayer(info[0].As<Napi::Number>().DoubleValue(), info[1].As<Napi::Number>().DoubleValue(), info[2].As<Napi::String>().Utf8Value());
	return number;
}

Napi::Number removeWrapped(const Napi::CallbackInfo& info)
{
	Napi::Env env = info.Env();
	auto number = Napi::Number::New(env, 0);
	if (info[0].IsString()) world.removePlayer(info[0].As<Napi::String>().Utf8Value());
	return number;
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
	exports.Set("step", Napi::Function::New(env, stepWrapped));
	exports.Set("add", Napi::Function::New(env, addWrapped));
	exports.Set("move", Napi::Function::New(env, accelWrapped));
	exports.Set("remove", Napi::Function::New(env, removeWrapped));

	return exports;
}

NODE_API_MODULE(testaddon, Init);