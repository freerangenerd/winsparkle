/*
 *  This file is part of WinSparkle (http://winsparkle.org)
 *
 *  Copyright (C) 2009-2013 Vaclav Slavik
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a
 *  copy of this software and associated documentation files (the "Software"),
 *  to deal in the Software without restriction, including without limitation
 *  the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *  and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *  DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef _appcast_h_
#define _appcast_h_

#include <string>

namespace winsparkle
{

/**
    This class contains information from the appcast.
 */
struct Appcast
{
    /// App version fields
    std::string Version;
    std::string ShortVersionString;

    /// URL of the update
    std::string DownloadURL;

    /**
		URL of the release notes page

		If localized release notes are provided with the lang attribute
		matching the preferred locale specified by the host, that link
		will be used.
	*/
    std::string ReleaseNotesURL;

    /// Title of the update
    std::string Title;

    /// Description of the update
    std::string Description;

	/** 
		Flag indicating that the available update is a paid update.

		An item specified in the appcast can be marked as a paid update by specifying a paidUpdate attribute
		whose value is the url for a web page to load in the user's default web browser, instead an an url
		attribute for an executable to download. The UI will tell the user a paid upgrade is available and
		instead of an Install button a Learn More button is presented. A paid update item can also display
		release notes. A user may elect to Skip This Version for a paid update. If the user skips the paid update
		and a free update is available, the next time WinSparkle checks for an update it will display the free
		update available for installation.
	*/
	bool IsPaidUpdate;

	/**
		Ptrs to child and parent updates available.

		A child exists for each entry in the feed following root
		root is the first entry read. Roots child is the next entry read,
		and so on. The end entry has no child.

		2013-11-06 - Presently, all entries are acquired, but only the first entry is used,
		unless the first entry is a paid update the user elected to skip and the second is a free update.
		In the future it would be nice to have a callback that lets the app do more sophisticated
		Appcast selection, akin to Sparkle on Mac's delegate method that lets the app select the best entry.
	*/
	struct Appcast *Child;
	struct Appcast *Parent; // weak reference

	// default constructor
	Appcast() : IsPaidUpdate(false), Child(NULL), Parent(NULL) {}

	// copy constructor
	Appcast(const struct Appcast& appcast) {
		*this = appcast;
	}

	// destructor
	~Appcast() { delete Child; }

	 // overload operator = for copy
	Appcast & operator=(const struct Appcast& appcast)
	{
		Version = appcast.Version;
		ShortVersionString = appcast.ShortVersionString;
		DownloadURL = appcast.DownloadURL;
		ReleaseNotesURL = appcast.ReleaseNotesURL;
		Title = appcast.Title;
		Description = appcast.Description;
		Parent = appcast.Parent;
		IsPaidUpdate = appcast.IsPaidUpdate;

		// deep copy children
		Child = NULL;
	
		if (appcast.Child) {
			Child = new struct Appcast(*(appcast.Child));
		}

		return *this;
	}

    /**
        Initializes the struct with data from XML appcast feed.

        If the feed contains multiple entries, only the latest one is read,
        the rest is ignored.

        Throws on error.

        @param xml Appcast feed data.
     */
    void Load(const std::string& xml);
};

} // namespace winsparkle

#endif // _appcast_h_
