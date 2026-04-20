//simon parker

/*
  TODO:
    -func that calculates the power for a test frequency
    -func that reads in wav data
    -func that reads a block of samples and deciphers the bit
    -func that takes in 10 blocks of samples and deciphers the byte
    -func that writes a byte to a file (does this need to be its own function?)


    code flow:
    read in 1 byte of data from the wav file (1600 samples, however many bytes of data that is (16 bits per sample, mono))
    for each bit in that byte (+2 for the start/stop), test which frequencies are dominant in that block (160 samples)
    write the decoded byte to the txt file
    print out the decoded byte
*/
