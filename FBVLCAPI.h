/**********************************************************\

  Auto-generated FBVLCAPI.h

\**********************************************************/

#include <string>
#include <sstream>
#include <boost/weak_ptr.hpp>
#include "JSAPIAuto.h"
#include "BrowserHost.h"
#include "FBVLC.h"

#ifndef H_FBVLCAPI
#define H_FBVLCAPI

class FBVLCAPI : public FB::JSAPIAuto
{
public:
    ////////////////////////////////////////////////////////////////////////////
    /// @fn FBVLCAPI::FBVLCAPI(const FBVLCPtr& plugin, const FB::BrowserHostPtr host)
    ///
    /// @brief  Constructor for your JSAPI object.
    ///         You should register your methods, properties, and events
    ///         that should be accessible to Javascript from here.
    ///
    /// @see FB::JSAPIAuto::registerMethod
    /// @see FB::JSAPIAuto::registerProperty
    /// @see FB::JSAPIAuto::registerEvent
    ////////////////////////////////////////////////////////////////////////////
    FBVLCAPI(const FBVLCPtr& plugin, const FB::BrowserHostPtr& host) :
        m_plugin(plugin), m_host(host)
    {
        // Read-only property
        registerProperty("version",
                         make_property(this,
                                       &FBVLCAPI::get_version));

        registerMethod("play",            make_method(this, &FBVLCAPI::play));
        registerMethod("togglePause",     make_method(this, &FBVLCAPI::togglePause));
        registerMethod("stop",            make_method(this, &FBVLCAPI::stop));
        registerMethod("toggleMute",      make_method(this, &FBVLCAPI::toggleMute));
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// @fn FBVLCAPI::~FBVLCAPI()
    ///
    /// @brief  Destructor.  Remember that this object will not be released until
    ///         the browser is done with it; this will almost definitely be after
    ///         the plugin is released.
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~FBVLCAPI() {};

    FBVLCPtr getPlugin();

    // Read-only property ${PROPERTY.ident}
    std::string get_version();

    void play(const std::string& mrl);
    void togglePause();
    void stop();

    void toggleMute();

private:
    FBVLCWeakPtr m_plugin;
    FB::BrowserHostPtr m_host;

};

#endif // H_FBVLCAPI

