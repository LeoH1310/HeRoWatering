// 
// 
// 

#include "EmailMangager.h"
#include "Logging.h"


bool sendEmail(char* subject, char* message, char* mailAddress) {
	// check if WiFi connection is established, if not reconnect
	checkWifiConnection();

	EMailSender* emailSender = new EMailSender(MAIL, MAILPASS);

	EMailSender::EMailMessage email;
	email.subject = subject;
	email.message = message;
	
	try {
		EMailSender::Response resp = emailSender->send(mailAddress, email);
		if (resp.status) myPrintln("Email sent.");
		else myPrintln("Error sending email. ");
	}
	catch (...) {
		myPrintln("Error sending email. ");
		return false;
	}

	delete emailSender;

	return true;
}
