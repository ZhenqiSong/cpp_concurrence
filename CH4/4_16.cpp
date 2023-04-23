//
// Created by Zhenqi Song on 2023/4/18.
// 状态函数getting_pin(),用于自动柜员机的简单实现

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


void atm::getting_pin(){
    incoming.wait().handle<digit_pressed>( // 处理按键消息
            [&](digit_pressed const& msg){
                unsigned const pin_length =4;
                pin += msg.digit;
                if (pin.length() == pin_length){
                    // 只有输入4位密码，才发送消息
                    bank.send(verigy_pin(account, pin, incoming));
                    state = &atm::verifying_pin;
                }
            }
            ).handle<clear_last_pressed>(
                    [&](clear_last_pressed const& msg){
                        if (!pin.empty()){
                            pin.resize(pin.length() - 1);
                        }
                    }
                    ).handle<cancel_pressed>(
                            [&](cancel_pressed const& msg){
                                state = &atm::done_processing;
                            }
                            )
}
