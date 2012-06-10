
#ifdef v8


string tostring(const v8::String::Utf8Value& value)
{
	return *value ? string(*value) : string("<string conversion failed>");
}

v8::Handle<v8::Value> LogCallback(const v8::Arguments& args)
{
	if (args.Length() < 1) return v8::Undefined();
	v8::HandleScope scope;
	v8::Handle<v8::Value> arg = args[0];
	v8::String::Utf8Value value(arg);
	cout << tostring(value) << endl;
	return v8::Undefined();
}

v8::Handle<v8::Value> registerAfterStart(const v8::Arguments& args)
{
	if (args.Length() < 1) return v8::Undefined();
	v8::HandleScope scope;
	v8::Handle<v8::Value> arg = args[0];
	if (!arg->IsFunction())
	{
		cout << "must register a function" << endl;
		return v8::Undefined();
	}
	v8::Handle<v8::Function> fun = v8::Handle<v8::Function>::Cast(arg);
	afterstart.push_back(v8::Persistent<v8::Function>::New(fun));
	cout << "registered." << endl;
	return v8::Undefined();
}



v8::Persistent<v8::Context> *js;

void init_v8()
{
	v8::HandleScope handle_scope;
	v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New();
	global->Set(v8::String::New("log"), v8::FunctionTemplate::New(LogCallback));
	global->Set(v8::String::New("registerafterstart"),
			v8::FunctionTemplate::New(registerAfterStart));
	*js = v8::Context::New(NULL, global);
	v8::Context::Scope context_scope(*js);
	ifstream in(jsv8.c_str());

	if (!in.fail())
	{
		string iin, line;
		while (in.good())
		{
			getline(in, line);
			iin += line;
		}

		// run honey run
		v8::Handle<v8::String> source = v8::String::New(iin.c_str());
		v8::Handle<v8::Script> script = v8::Script::Compile(source);
		v8::Handle<v8::Value> result = script->Run();
	}
}


void kill_v8()
{
	js->Dispose();
}
#endif

