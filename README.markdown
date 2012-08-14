# iprange
By Robert Massaioli (robertmassaioli@gmail.com)

## Purpose

The purpose of this library is to provide a way to add ranges of IP's to an object
and then retrieve wether or not an IP address is in that range. This project is supposed
to be robust and be able to handle bad input in nice ways as well as being easily able to
hande both IPv4 and IPv6 adresses. (NOTE: Currently only IPv4 adresses are supported)

## Adding to a project

Currently just make sure that iprange.(tcc|h) are in your include path and say:

    #include <iprange.h>

Then when you make the class you have to call:

    IPRange<IPV4_t> myRangeObject;

When you want to add ip adresses:

    // this one example includes every possible range type you can specify
    myRangeObject.add("192.*.[0-10].[100-*]");

When you want to check is a single IP address is in and range you have added:

    if(myRangeObject.includes("192.168.0.199")) {
       cout << "Hey, it's included!" << endl;
    } else {
       cout << "Dang, it's not." << endl;
    }

And that's all there is to it.

## Functions Provided

 - add         | add a new IP range
 - includes    | is an ip included in any of the ranges
 - size        | how many ranges does the object have
 - clear       | remove and free all ranges from the object
 - <<          | output the object to an ostream

# Technical Details

The search through the IP adresses is linear; working on making it binary
search in the near future.
