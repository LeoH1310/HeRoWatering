// emailMangager.h

#pragma once

#include "arduino.h"

// Email Library
#include <EMailSender.h>

extern const char* MAIL;
extern const char* MAILPASS;

bool sendEmail(char* subject, char* message, char* mailAddress);
