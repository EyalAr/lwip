The module 'decree' takes care of making sure the arguments are legal and have
correct types; and makes sure to disambiguate them. If there is a problem with
the arguments (regarding types and order), decree will throw an error.
So we don't need to test that here.
What we do need to test that decree doesn't - is if the arguments are legal,
but just don't make sense.
For example, if an argument needs to be a string from some set of strings, this
is something we need to check.
