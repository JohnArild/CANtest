class BSP_BluePill {
    public:
        int initilize();
    private:
        int MCUspeed_48MHz();
        int setup_GPIO();
        int setup_CAN();
        int setup_SPI();
};