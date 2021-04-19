// 
// 
// 

#include "EmailMangager.h"

bool sendEmail(char* subject, char* message, char* mailAddress) {
	EMailSender::EMailMessage email;
	email.subject = subject;
	email.message = message;
		
	EMailSender::Response resp = emailSender.send(mailAddress, email);

	if (resp.status) {
		Serial.println("Email sent.");
	}
	else {
		Serial.println("Error sending email.");
	}

	return resp.status;
}
