//
// Created by Zhenqi Song on 2023/4/13.
// atm类的简单实现，其功能是模拟自动柜员机的业务逻辑

#include <string>

struct card_inserted{
    std::string account;
};

class atm{
    messaging::reveiver incoming;
    messaging::sender bank;
    messaging::sender interface_hardware;

    void (atm::*state)();
    std::string account;
    std::string pin;

    void waiting_for_card(){
        interface_hardware.send(display_enter_card());

        incoming.wait().handle<card_inserted>([&](card_inserted const& msg){
            account = msg.account;
            pin = "";
            interface_hardware.send(display_enter_pin());
            state = &atm::getting_pin;
        };
    }

    void getting_pin();

public:
    void run(){
        state = &atm::waiting_for_card;
        try{
            for(;;){
                (this->*state)();
            }
        }catch (messaging::close_queue const&){

        }
    }
};
