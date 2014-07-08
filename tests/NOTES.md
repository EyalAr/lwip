# Note about arguments validation (test batch 00)

The module 'decree' takes care of making sure the arguments are legal and have
correct types; and makes sure to disambiguate them. If there is a problem with
the arguments (regarding types and order), decree will throw an error.
So we don't need to test that here.
What we do need to test that decree doesn't - is if the arguments are legal,
but just don't make sense.
For example, if an argument needs to be a string from some set of strings, this
is something we need to check.

# Note about image operations (test batch 02)

Testing image operations has two purposes:

0. Making sure no runtime errors occur during the operation.
0. Making sure the operation does what it's supposed to do.

The tests here will (hopefully) catch any unexpected errors. But we can't
automatically verify the actual results are correct.

For this, we need to manually look at the images and make sure they make sense.

Each test unit will generate an image with a descriptive name. For example,
one of the `resize` tests will create a file `resize_1000X1330_lanczos.jpg`,
which indicates this is a resize image to 1000X1330 pixels, using lanczos
interpolation.

A human will need to manually go over the images and make sure they are indeed
what their names describe.

All images will be stored in `../results/`.
