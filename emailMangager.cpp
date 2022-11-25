// 
// 
// 

#include "Credentials.h"
#include "EmailMangager.h"
#include "Logging.h"


bool sendEmail(char* subject, char* message, char* mailAddress) {
	// check if WiFi connection is established, if not reconnect
	checkWifiConnection();

	EMailSender* emailSender = new EMailSender(MAIL_SENDER, MAIL_PASS);
	//EMailSender* emailSender = new EMailSender(MAIL_SENDER, MAIL_PASS, MAIL_SENDER, MAIL_SMTP_SERVER, MAIL_SMTP_PORT);

	EMailSender::EMailMessage email;
	email.subject = subject;
	email.message = message;
	
	try {
		EMailSender::Response resp = emailSender->send(mailAddress, email);
		if (resp.status) myPrintln("Email sent.");
		else {
			myPrintln("Error sending email. ");
			Serial.println("Sending status: ");
			Serial.println(resp.code);
			Serial.println(resp.desc);
			Serial.println(resp.status);
		}
	}
	catch (...) {
		myPrintln("Error sending email. ");
		return false;
	}

	delete emailSender;

	return true;
}
