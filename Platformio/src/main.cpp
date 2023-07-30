#include <Arduino.h>

#include "Message.h"
#include "Characters.h"

static Message message;

void setup(){
    using namespace characters;

    message.init();
    message.addImage(H);
    message.addImage(I);
    message.addImage(R);
    message.addImage(E);
    message.addImage(SPACE);
    message.addImage(M);
    message.addImage(E);
    message.addImage(EXCLAMATION);
}

void loop(){
    message.playMessage(500, true, true);
    delay(1000);
}

