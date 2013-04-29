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
#include "Common.h"

#include "Messages.h"

// Must be included after Common.h or errors will ensue...
#include <map>
#include <vector>

using namespace std;

namespace Messages
{
	map<const string, vector<Recipient*> > g_recipients;

	void deregisterAll()
	{
		g_recipients.clear();
	}

	void registerRecipient(const string& _sSubject, Recipient* _pRecipient)
	{
		g_recipients[_sSubject].push_back(_pRecipient);
	}

	void send(const string& _sSubject, const void* _pMessage)
	{
		if (g_recipients.find(_sSubject) == g_recipients.end())
		{
			return;
		}

		vector<Recipient*>& registeredRecipients = g_recipients[_sSubject];

		for (unsigned int uiIndex = 0; uiIndex < registeredRecipients.size(); uiIndex++)
		{
			registeredRecipients[uiIndex]->Receive(_sSubject, _pMessage);
		}
	}
}
