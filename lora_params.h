struct LoraParams {
    // User-configurable parameters
    int tp = -1;       // Transmission power
    int sf = -1;       // Spreading factor
    int cr = -1;       // Coding rate
    int pl = -1;       // Preamble length
    int sw = -1;       // Sync word
    long fr = -1;      // Frequency
    long bw = -1;      // Bandwidth

    // Internal-only flags (not sent in config messages)
    bool crc = true;        // Use CRC in LoRa packet
    bool invertIQ = false;  // IQ inversion for gateway compatibility
    bool ldro = false;      // Low data rate optimization
};
