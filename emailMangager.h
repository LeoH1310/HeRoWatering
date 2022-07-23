// emailMangager.h

#pragma once

#include "arduino.h"

// Email Library
#include <EMailSender.h>

bool sendEmail(char* subject, char* message, char* mailAddress);
