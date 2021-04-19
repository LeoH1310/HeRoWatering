// emailMangager.h

#pragma once

#include "arduino.h"

// Email Library
#include <EMailSender.h>

extern EMailSender emailSender;
extern const char* MAIL;
extern const char* MAILPASS;

void connectEmail();
bool sendEmail(char* subject, char* message, char* mailAddress);
