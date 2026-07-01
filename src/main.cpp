int main(void)
{
    Server ser;

    try
    {
        signal(SIGINT, Server::signalHandler);
        signal(SIGQUIT, Server::signalHandler);
        ser.serverInit();
    }
    catch()
    {
        ser.closeFds();
        std::cerr << e.what() << std::endl;
    }
    std::cout << ">> Server closed <<" << std::endl;
}