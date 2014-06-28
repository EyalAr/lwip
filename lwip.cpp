#include <node.h>
#include <v8.h>

using namespace v8;

// create a function templates
Handle<Value> getWorld(const Arguments &args){
    // this scope will discard all internal local objects for us.
    HandleScope scope;

    // indicates if the callback had been called
    Handle<Boolean> result = Boolean::New(false);

    // arguments validation:
    if (args.Length() >= 1 && args[0]->IsFunction()){
        // cast the first argument to a function and create a function object
        Local<Function> cb = Local<Function>::Cast(args[0]);
        // define the arguments for the callback
        const unsigned int argc = 1;
        Local<Value> argv[argc] = { Local<Value>::New(String::New("world")) };
        // run the callback
        cb->Call(Context::GetCurrent()->Global(),argc,argv);
        result = Boolean::New(true);
    }

    // we must use scope.Close in order to make the return object persistent
    // outside the scope of this function.
    return scope.Close(result);
}

// create an init function for our node module
// this function is equivalent to initializing the 'exports' object
// in a node javascript module.
void init(Handle<Object> exports, Handle<Object> module){
    // equivalent to: module.exports = getWorld
    module->Set(String::NewSymbol("exports"),
        FunctionTemplate::New(getWorld)->GetFunction());
}

// use NODE_MODULE macro to register our module:
NODE_MODULE(lwip,init)
