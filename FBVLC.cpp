/**********************************************************\

  Auto-generated FBVLC.cpp

  This file contains the auto-generated main plugin object
  implementation for the FireBreathed VLC project

\**********************************************************/

#include "FBVLCAPI.h"

#include "FBVLC.h"

///////////////////////////////////////////////////////////////////////////////
/// @fn FBVLC::StaticInitialize()
///
/// @brief  Called from PluginFactory::globalPluginInitialize()
///
/// @see FB::FactoryBase::globalPluginInitialize
///////////////////////////////////////////////////////////////////////////////
void FBVLC::StaticInitialize()
{
    // Place one-time initialization stuff here; As of FireBreath 1.4 this should only
    // be called once per process
}

///////////////////////////////////////////////////////////////////////////////
/// @fn FBVLC::StaticInitialize()
///
/// @brief  Called from PluginFactory::globalPluginDeinitialize()
///
/// @see FB::FactoryBase::globalPluginDeinitialize
///////////////////////////////////////////////////////////////////////////////
void FBVLC::StaticDeinitialize()
{
    // Place one-time deinitialization stuff here. As of FireBreath 1.4 this should
    // always be called just before the plugin library is unloaded
}

///////////////////////////////////////////////////////////////////////////////
/// @brief  FBVLC constructor.  Note that your API is not available
///         at this point, nor the window.  For best results wait to use
///         the JSAPI object until the onPluginReady method is called
///////////////////////////////////////////////////////////////////////////////
FBVLC::FBVLC()
    : m_libvlc(0), vlc::vmem( this->basic_player() )
{
}

///////////////////////////////////////////////////////////////////////////////
/// @brief  FBVLC destructor.
///////////////////////////////////////////////////////////////////////////////
FBVLC::~FBVLC()
{
    // This is optional, but if you reset m_api (the shared_ptr to your JSAPI
    // root object) and tell the host to free the retained JSAPI objects then
    // unless you are holding another shared_ptr reference to your JSAPI object
    // they will be released here.
    releaseRootJSAPI();
    m_host->freeRetainedObjects();
}

//libvlc events arrives from separate thread
void FBVLC::OnLibVlcEvent_proxy(const libvlc_event_t* e, void *param)
{
    FBVLC* fbvlc = static_cast<FBVLC*>(param);

    FBVLCAPIPtr api  = boost::static_pointer_cast<FBVLCAPI>( fbvlc->getRootJSAPI() );
    FB::BrowserHostPtr h = fbvlc->m_host;

    void (FBVLCAPI::*event_to_fire)(void) = 0;

    switch ( e->type ) {
    case libvlc_MediaPlayerMediaChanged:
        event_to_fire = &FBVLCAPI::fire_MediaPlayerMediaChanged;
        break;
    case libvlc_MediaPlayerNothingSpecial:
        event_to_fire = &FBVLCAPI::fire_MediaPlayerNothingSpecial;
        break;
    case libvlc_MediaPlayerOpening:
        event_to_fire = &FBVLCAPI::fire_MediaPlayerOpening;
        break;
    case libvlc_MediaPlayerBuffering:
        event_to_fire = &FBVLCAPI::fire_MediaPlayerBuffering;
        break;
    case libvlc_MediaPlayerPlaying:
        event_to_fire = &FBVLCAPI::fire_MediaPlayerPlaying;
        break;
    case libvlc_MediaPlayerPaused:
        event_to_fire = &FBVLCAPI::fire_MediaPlayerPaused;
        break;
    case libvlc_MediaPlayerStopped:
        event_to_fire = &FBVLCAPI::fire_MediaPlayerStopped;
        break;
    case libvlc_MediaPlayerForward:
        event_to_fire = &FBVLCAPI::fire_MediaPlayerForward;
        break;
    case libvlc_MediaPlayerBackward:
        event_to_fire = &FBVLCAPI::fire_MediaPlayerBackward;
        break;
    case libvlc_MediaPlayerEndReached:
        event_to_fire = &FBVLCAPI::fire_MediaPlayerEndReached;
        break;
    case libvlc_MediaPlayerEncounteredError:
        event_to_fire = &FBVLCAPI::fire_MediaPlayerEncounteredError;
        break;
    case libvlc_MediaPlayerTimeChanged:
        event_to_fire = &FBVLCAPI::fire_MediaPlayerTimeChanged;
        break;
    case libvlc_MediaPlayerPositionChanged:
        event_to_fire = &FBVLCAPI::fire_MediaPlayerPositionChanged;
        break;
    case libvlc_MediaPlayerSeekableChanged:
        event_to_fire = &FBVLCAPI::fire_MediaPlayerSeekableChanged;
        break;
    case libvlc_MediaPlayerPausableChanged:
        event_to_fire = &FBVLCAPI::fire_MediaPlayerPausableChanged;
        break;
    //case libvlc_MediaPlayerTitleChanged:
    //    event_to_fire = &FBVLCAPI::fire_MediaPlayerTitleChanged;
    //    break;
    //case libvlc_MediaPlayerSnapshotTaken:
    //    event_to_fire = &FBVLCAPI::fire_MediaPlayerSnapshotTaken;
    //    break;
    //case libvlc_MediaPlayerLengthChanged:
    //    event_to_fire = &FBVLCAPI::fire_MediaPlayerLengthChanged;
    //    break;
    //case libvlc_MediaPlayerVout:
    //    event_to_fire = &FBVLCAPI::fire_MediaPlayerVout;
    //    break;
    };

    if ( event_to_fire ) {
        h->ScheduleOnMainThread( api, boost::bind( event_to_fire, api.get() ) );
    }
}

void FBVLC::VlcEvents(bool Attach)
{
    if ( !get_player().is_open() )
        return;

    libvlc_event_manager_t* em =
        libvlc_media_player_event_manager( get_player().get_mp() );
    if(!em)
        return;

    for(int e=libvlc_MediaPlayerMediaChanged; e<=libvlc_MediaPlayerVout; ++e){
        switch(e){
        case libvlc_MediaPlayerMediaChanged:
        case libvlc_MediaPlayerNothingSpecial:
        case libvlc_MediaPlayerOpening:
        case libvlc_MediaPlayerBuffering:
        case libvlc_MediaPlayerPlaying:
        case libvlc_MediaPlayerPaused:
        case libvlc_MediaPlayerStopped:
        case libvlc_MediaPlayerForward:
        case libvlc_MediaPlayerBackward:
        case libvlc_MediaPlayerEndReached:
        case libvlc_MediaPlayerEncounteredError:
        case libvlc_MediaPlayerTimeChanged:
        case libvlc_MediaPlayerPositionChanged:
        case libvlc_MediaPlayerSeekableChanged:
        case libvlc_MediaPlayerPausableChanged:
        //case libvlc_MediaPlayerTitleChanged:
        //case libvlc_MediaPlayerSnapshotTaken:
        //case libvlc_MediaPlayerLengthChanged:
        //case libvlc_MediaPlayerVout:
            if(Attach)
                libvlc_event_attach(em, e, OnLibVlcEvent_proxy, this);
            else
                libvlc_event_detach(em, e, OnLibVlcEvent_proxy, this);
            break;
        }
    }
}

const FB::variant& FBVLC::getParamVariant(const std::string& key) const
{
    FB::VariantMap::const_iterator fnd = m_params.find(key.c_str());
    if (fnd != m_params.end())
        return fnd->second;

    static const FB::variant empty;
    return empty;
}

void FBVLC::init_vlc_player_options()
{
    typedef boost::optional<std::string> param_type;
    typedef const FB::variant&           param_vtype;

    vlc_player_options& opts = get_options();

    param_vtype autoplay        = getParamVariant("autoplay");
    param_vtype autostart       = getParamVariant("autostart");

    if ( !autoplay.empty() && autoplay.can_be_type<bool>() )
        opts.set_autoplay( autoplay.convert_cast<bool>() );
    if ( !autostart.empty() && autostart.can_be_type<bool>() )
        opts.set_autoplay( autostart.convert_cast<bool>() );

    param_vtype fs              = getParamVariant("fullscreen");
    param_vtype allowfs         = getParamVariant("allowfullscreen");
    param_vtype allowfs2        = getParamVariant("allow-fullscreen");
    param_vtype fsenabled       = getParamVariant("fullscreenenabled");
    param_vtype fsenabled2      = getParamVariant("fullscreen-enabled");
    if ( !fs.empty() && fs.can_be_type<bool>() )
        opts.set_enable_fs( fs.convert_cast<bool>() );
    if ( !allowfs.empty() && allowfs.can_be_type<bool>() )
        opts.set_enable_fs( allowfs.convert_cast<bool>() );
    if ( !allowfs2.empty() && allowfs2.can_be_type<bool>() )
        opts.set_enable_fs( allowfs2.convert_cast<bool>() );
    if ( !fsenabled.empty() && fsenabled.can_be_type<bool>() )
        opts.set_enable_fs( fsenabled.convert_cast<bool>() );
    if ( !fsenabled2.empty() && fsenabled2.can_be_type<bool>() )
        opts.set_enable_fs( fsenabled2.convert_cast<bool>() );

    param_vtype toolbar         = getParamVariant("toolbar");
    if ( !toolbar.empty() && toolbar.can_be_type<bool>() )
        opts.set_show_toolbar( toolbar.convert_cast<bool>() );

    param_vtype fs_toolbar      = getParamVariant("fullscreen-toolbar");
    if ( !fs_toolbar.empty() && fs_toolbar.can_be_type<bool>() )
        opts.set_show_fs_toolbar( fs_toolbar.convert_cast<bool>() );

#ifdef XP_WIN
    param_vtype native_scaling  = getParamVariant("native-scaling");
    if ( !native_scaling.empty() && native_scaling.can_be_type<bool>() )
        opts.set_native_scaling ( native_scaling.convert_cast<bool>() );
#endif

    param_type bgcolor          = getParam("bgcolor");
    if ( bgcolor )
        opts.set_bg_color( *bgcolor );

    param_vtype use_proxy = getParamVariant("use-proxy");
    if ( !use_proxy.empty() && use_proxy.can_be_type<bool>() )
        opts.set_use_proxy( use_proxy.convert_cast<bool>() );
}

void FBVLC::init_libvlc_options( std::vector<std::string>* opts)
{
    if( !opts )
        return;

    typedef boost::optional<std::string> param_type;
    typedef const FB::variant&           param_vtype;

    param_type network_caching = getParam("network-caching");
    if ( network_caching )
    {
        opts->push_back( "--network-caching" );
        opts->push_back( *network_caching );
    };

    param_vtype adjust         = getParamVariant("adjust-filter");
    if ( !adjust.empty() && adjust.can_be_type<bool>() && adjust.convert_cast<bool>() ) {
        opts->push_back( "--video-filter=adjust" );
    }

    std::string sub_filters;
    param_vtype marq           = getParamVariant("marquee-filter");
    if ( !marq.empty() && marq.can_be_type<bool>() && marq.convert_cast<bool>() ) {
        sub_filters = "marq";
    }

    param_vtype logo           = getParamVariant("logo-filter");
    if ( !logo.empty() && logo.can_be_type<bool>() && logo.convert_cast<bool>() ) {
        if( !sub_filters.empty() )
            sub_filters += ':';
        sub_filters += "logo";
    }

    if( !sub_filters.empty() ) {
        opts->push_back( "--sub-filter=" + sub_filters );
    }

    param_vtype debug          = getParamVariant("debug");
    if ( !debug.empty() && debug.can_be_type<bool>() && debug.convert_cast<bool>() ) {
        opts->push_back( "-vvv" );
    }

    param_vtype hw_accel         = getParamVariant("hw-accel");
    if ( !hw_accel.empty() && hw_accel.can_be_type<bool>() && hw_accel.convert_cast<bool>() ) {
        const char* version = libvlc_get_version();
        if( 0 == strncmp(version, "2.0.", 4) )
            opts->push_back( "--ffmpeg-hw" ); //for vlc 2.0
    }

    /*** add new libvlc options here ***/

}

void FBVLC::process_startup_options()
{
    typedef boost::optional<std::string> param_type;
    typedef const FB::variant&           param_vtype;

    vlc_player_options& opts = get_options();

    param_vtype mute            = getParamVariant("mute");
    if ( !mute.empty() && mute.can_be_type<bool>() )
        get_player().audio().set_mute( mute.convert_cast<bool>() );

    param_vtype loop            = getParamVariant("loop");
    param_vtype autoloop        = getParamVariant("autoloop");
    bool set_loop = false;
    if ( !loop.empty() && loop.can_be_type<bool>() )
        set_loop = loop.convert_cast<bool>();
    if ( !autoloop.empty() && autoloop.can_be_type<bool>() )
        set_loop = autoloop.convert_cast<bool>();
    get_player().set_playback_mode( set_loop ?
                                    vlc::mode_loop :
                                    vlc::mode_normal );

    param_type target           = getParam("target");
    param_type mrl              = getParam("mrl");
    param_type filename         = getParam("filename");
    param_type src              = getParam("src");
    std::string set_mrl;
    if ( target )
        set_mrl = *target;
    if ( mrl )
        set_mrl = *mrl;
    if ( filename )
        set_mrl = *filename;
    if ( src )
        set_mrl = *src;
    if( !set_mrl.empty() ) {
        int item = add_playlist_item( set_mrl.c_str() );
        if ( opts.get_autoplay() )
            get_player().play(item);
    }
}

void FBVLC::vlc_open()
{
    if( get_player().is_open() )
        return ;

    init_vlc_player_options();

    if( !m_libvlc ) {
        /* prepare VLC command line */
        std::vector<std::string> libvlc_options;
        init_libvlc_options( &libvlc_options );

        std::vector<const char*> libvlc_c_opts;
        libvlc_c_opts.push_back("--no-video-title-show");
        /*** add static libvlc options here ***/

        std::vector<std::string>::const_iterator i     = libvlc_options.begin();
        std::vector<std::string>::const_iterator end_i = libvlc_options.end();
        for( ; i != end_i; ++i ) {
            libvlc_c_opts.push_back( i->c_str() );
        }

        m_libvlc = libvlc_new( libvlc_c_opts.size(),
                               libvlc_c_opts.empty() ? 0 : &libvlc_c_opts[0] );
    }

    if ( m_libvlc && !get_player().is_open() ) {
        get_player().open(m_libvlc);
        VlcEvents(true);
    }

    if ( get_player().is_open() && isWindowless() ) {
        vlc::vmem::open();
    }

    process_startup_options();
}

void FBVLC::vlc_close()
{
    get_player().stop();

    if ( get_player().is_open() && isWindowless() ) {
        vlc::vmem::close();
    }

    if ( get_player().is_open() ) {
        VlcEvents(false);
        get_player().close();
    }

    if ( m_libvlc ) {
        libvlc_free(m_libvlc);
        m_libvlc = 0;
    }
}

std::string FBVLC::detectHttpProxy( const std::string& mrl ) const
{
    //detect proxy from browser
    typedef std::map<std::string, std::string> proxyInfo_t;
    typedef proxyInfo_t::const_iterator proxyInfo_it;
    proxyInfo_t proxyInfo;

    std::string proxy_str;
    if( get_options().get_use_proxy() && m_host->DetectProxySettings( proxyInfo, mrl ) ) {
        proxyInfo_it typeIt     = proxyInfo.find("type");
        proxyInfo_it hostNameIt = proxyInfo.find("hostname");
        proxyInfo_it portIt     = proxyInfo.find("port");

        if( proxyInfo.end() != typeIt && "http" == typeIt->second &&
            proxyInfo.end() != hostNameIt && !hostNameIt->second.empty() &&
            proxyInfo.end() != portIt && !portIt->second.empty() )
        {
            proxy_str = ":http-proxy=" + hostNameIt->second + ":" + portIt->second;
        }
    }

    return proxy_str;
}

const std::string trustedOptions[] = {
    ":rtsp-http-port=",
};

bool FBVLC::isTrustedOption( const std::string& option )
{
    const unsigned trustedCount = sizeof( trustedOptions ) / sizeof( trustedOptions[0] );
    for( unsigned i = 0; i < trustedCount; ++i ) {
        if( 0 == option.compare(0, trustedOptions[i].size(), trustedOptions[i] ) )
            return true;
    }

    return false;
}

int FBVLC::add_playlist_item( const std::string& mrl )
{
    return get_player().add_media( mrl.c_str() );
}

int FBVLC::add_playlist_item( const std::string& mrl, const std::vector<std::string>& options )
{
    const std::string proxy_str = detectHttpProxy( mrl );

    //trusted options
    std::vector<const char*> trusted_opts;
    if( !proxy_str.empty() ) {
        trusted_opts.push_back( proxy_str.c_str() );
    }

    //untrusted options
    std::vector<const char*> untrusted_opts;
    for( unsigned i = 0; i < options.size(); ++i ) {
        if( isTrustedOption( options[i] ) )
            trusted_opts.push_back( options[i].c_str() );
        else
            untrusted_opts.push_back( options[i].c_str() );
    }

    /*another trusted and untrusted options could be added here*/

    const char** untrusted_optv = untrusted_opts.empty() ? 0 : &untrusted_opts[0];
    const char** trusted_optv = trusted_opts.empty() ? 0 : &trusted_opts[0];
    return get_player().add_media( mrl.c_str(),
                                   untrusted_opts.size(), untrusted_optv,
                                   trusted_opts.size(), trusted_optv );
}

void FBVLC::onPluginReady()
{
    // When this is called, the BrowserHost is attached, the JSAPI object is
    // created, and we are ready to interact with the page and such.  The
    // PluginWindow may or may not have already fire the AttachedEvent at
    // this point.
    vlc_open();
}

void FBVLC::shutdown()
{
    // This will be called when it is time for the plugin to shut down;
    // any threads or anything else that may hold a shared_ptr to this
    // object should be released here so that this object can be safely
    // destroyed. This is the last point that shared_from_this and weak_ptr
    // references to this object will be valid
    vlc_close();
}

///////////////////////////////////////////////////////////////////////////////
/// @brief  Creates an instance of the JSAPI object that provides your main
///         Javascript interface.
///
/// Note that m_host is your BrowserHost and shared_ptr returns a
/// FB::PluginCorePtr, which can be used to provide a
/// boost::weak_ptr<FBVLC> for your JSAPI class.
///
/// Be very careful where you hold a shared_ptr to your plugin class from,
/// as it could prevent your plugin class from getting destroyed properly.
///////////////////////////////////////////////////////////////////////////////
FB::JSAPIPtr FBVLC::createJSAPI()
{
    // m_host is the BrowserHost
    return boost::make_shared<FBVLCAPI>(FB::ptr_cast<FBVLC>(shared_from_this()), m_host);
}

/*
bool FBVLC::onMouseDown(FB::MouseDownEvent *evt, FB::PluginWindow *)
{
    //printf("Mouse down at: %d, %d\n", evt->m_x, evt->m_y);
    return false;
}

bool FBVLC::onMouseUp(FB::MouseUpEvent *evt, FB::PluginWindow *)
{
    //printf("Mouse up at: %d, %d\n", evt->m_x, evt->m_y);
    return false;
}

bool FBVLC::onMouseMove(FB::MouseMoveEvent *evt, FB::PluginWindow *)
{
    //printf("Mouse move at: %d, %d\n", evt->m_x, evt->m_y);
    return false;
}
*/

bool FBVLC::onWindowAttached(FB::AttachedEvent *evt, FB::PluginWindow *)
{
    vlc_open();
    return true;
}

bool FBVLC::onWindowDetached(FB::DetachedEvent *evt, FB::PluginWindow *)
{
    vlc_close();
    return true;
}

bool FBVLC::onWindowResized( FB::ResizedEvent *evt, FB::PluginWindow* w )
{
    if( isWindowless() ) {
        if( get_options().get_native_scaling() )
            vlc::vmem::set_desired_size( vlc::original_media_width, vlc::original_media_height );
        else
            vlc::vmem::set_desired_size( w->getWindowWidth(), w->getWindowHeight() );
    }

    return true;
}
