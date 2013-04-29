//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// Copyright (c) 2012 Media Design School
//
// Author: Gary Buyn
//
// SVN Info
// $Author: MEDIADESIGN\gary.buyn $
// $Date: 2013-01-23 09:40:30 +1300 (Wed, 23 Jan 2013) $
// $Rev: 1108 $
//
#ifndef MESSAGES_H_
#define MESSAGES_H_

#include "Common.h"

// Must be included after Common.h or errors will ensue...
#include <string>

namespace Messages
{
	class Recipient
	{
		public:
			virtual void Receive(const std::string& _sSubject, const void* _pMessage) = 0;
	};

	/**
	 * Deregisters all recipients from all subjects.
	 */
	void deregisterAll();

	/**
	 * Registers a recipient for the given subject. Standard simplicity events can be found in Events.h.
	 *
	 * @param subject The subject to register the recipient with.
	 * @param recipient The recipient to send messages of the given subject to.
	 */
	void registerRecipient(const std::string& _sSubject, Recipient* _pRecipient);

	/**
	 * Sends the given message to all registered recipients.
	 *
	 * @param subject The subject of the message.
	 * @param message The message to send.
	 */
	void send(const std::string& _sSubject, const void* _pMessage);
}

#endif /* MESSAGES_H_ */
