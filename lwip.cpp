#include <node.h>
#include <v8.h>

using namespace v8;

// create a function templates for a function which returns an object:
Handle<Value> getWorld(const Arguments &args){
    // this scope will discard all internal local objects for us.
    HandleScope scope;
    // we return a string object.
    // we must use scope.Close in order to make the string persistent
    // outside the scope of this function.
    return scope.Close(String::New("world"));
}

// create an init function for our node module
// this function is equivalent to initializing the 'exports' object
// in a node javascript module.
void init(Handle<Object> exports){
    // equivalent to: exports.hello = getWorld
    exports->Set(String::NewSymbol("hello"),
        FunctionTemplate::New(getWorld)->GetFunction());
}

// use NODE_MODULE macro to register our module:
NODE_MODULE(lwip,init)
