#pragma once
#include <string>

// this is a foundation wallet public key 
// to which the FOUNDATION_AMOUNT of coins will be "transferred" in the genesis block

// for the raw string of public key it is important to include \n as the last char
// otherwise publicKey comparison fails
const std::string FOUNDATION_PUBKEY =
    R"(-----BEGIN PUBLIC KEY-----
MFYwEAYHKoZIzj0CAQYFK4EEAAoDQgAEbqJNTPeXHOdqKbpFZwWjDyankHVx/TEO
tu2eMthSwBCHIHx0XiiSo3Jvyx2X/DnQOs9U3TNl2yQQmwfBaMlGsQ==
-----END PUBLIC KEY-----
)";


