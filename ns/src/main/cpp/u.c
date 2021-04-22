
/**
 * I'm writing a C function to calculate the XOR of all members in an array of bytes.
 * Each member should be a 512-byte block.
 * Here is what i'm figuring how to write this function.
 * the parameter includes the input array of 512-byte blocks
 * the return result should be an string (char* or char[512])
 * @param arr
 * @return
 */
char* calXOR(char* arr[512]) {
    char* xr;

    if (sizeof(arr) < 2) { // if there is only one segment payload
        xr = arr[0];
    }
    else {
        if (sizeof(arr) == 2) { // if there are payload from two segments
            xr = arr[0] ^ arr[1];
        }
        else {
            if (sizeof(arr) > 2) { // if there are payload from more than 2 segments
                xr = arr[0] ^ arr[1];
                int i;
                for (i = 2; i < sizeof(arr); i ++) {
                    xr = xr ^ arr[i];
                }
            }
        }
    }
    return xr;
}