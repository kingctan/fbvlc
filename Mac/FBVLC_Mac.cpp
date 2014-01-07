#include "FBVLC_Mac.h"

////////////////////////////////////////////////////////////////////////////////
//FBVLC_Mac class
////////////////////////////////////////////////////////////////////////////////
FBVLC_Mac::FBVLC_Mac()
{
    updateBgComponents();
}

FBVLC_Mac::~FBVLC_Mac()
{
}

void FBVLC_Mac::updateBgComponents()
{
    uint8_t r = 0, g = 0, b = 0;
    HtmlColor2RGB(get_options().get_bg_color(), &r, &g, &b);
    m_bgComponents[0] = r / 255.f;
    m_bgComponents[1] = g / 255.f;
    m_bgComponents[2] = b / 255.f;
    m_bgComponents[3] = 1.f;
}

void FBVLC_Mac::on_option_change(vlc_player_option_e option)
{
    FBVLC::on_option_change(option);

    switch (option) {
        case po_bg_color: {
            updateBgComponents();
            if ( GetWindow() )
                GetWindow()->InvalidateWindow();
            break;
        }
        default:
            break;
    }
}

void FBVLC_Mac::on_frame_ready( const std::vector<char>& frame_buf )
{
}

void FBVLC_Mac::on_frame_cleanup()
{
}

bool FBVLC_Mac::onCoreGraphicsDraw(FB::CoreGraphicsDraw *evt, FB::PluginWindowMacCG*)
{
    FB::Rect bounds(evt->bounds);
    //FB::Rect clip(evt->clip);
    short width = bounds.right - bounds.left, height = bounds.bottom - bounds.top;

    CGContextRef cgContext(evt->context);

    CGContextSaveGState(cgContext);

    CGContextTranslateCTM(cgContext, 0.0, height);
    CGContextScaleCTM(cgContext, 1.0, -1.0);

    CGColorSpaceRef cSpace = CGColorSpaceCreateDeviceRGB();
    CGContextSetFillColorSpace(cgContext, cSpace);

    CGColorRef bgColor = CGColorCreate(cSpace, m_bgComponents);
    CGContextSetFillColorWithColor(cgContext, bgColor);

    
    const std::vector<char>& fb = frame_buf();
    unsigned media_width = vlc::vmem::width();
    unsigned media_height = vlc::vmem::height();

    if ( fb.size() &&
         fb.size() >= media_width * media_height * vlc::DEF_PIXEL_BYTES )
    {
        CGContextRef bmpCtx =
            CGBitmapContextCreate( (void*)&fb[0], media_width, media_height, 8,
                                   media_width * vlc::DEF_PIXEL_BYTES, cSpace,
                                   kCGImageAlphaPremultipliedFirst | kCGBitmapByteOrder32Little );
        CGImageRef img = CGBitmapContextCreateImage(bmpCtx);

        CGRect imgRect = {
            { (width - media_width) / 2, (height - media_height) / 2 },
            { media_width, media_height }
        };
        CGContextDrawImage(cgContext, imgRect, img);

        CGImageRelease(img);
        CGContextRelease(bmpCtx);

        if( media_width < width ) {
            CGRect bgLeft = {
                { 0, 0 },
                { imgRect.origin.x, height }
            };
            CGContextFillRect(cgContext, bgLeft);

            CGRect bgRight = {
                { imgRect.origin.x + imgRect.size.width, 0 },
                { width - (imgRect.origin.x + imgRect.size.width), height }
            };
            CGContextFillRect(cgContext, bgRight);

        } else if( media_height < height ){
            CGRect bgTop = {
                { 0, 0 },
                { width, imgRect.origin.y }
            };
            CGContextFillRect(cgContext, bgTop);

            CGRect bgBottom = {
                { 0, imgRect.origin.y + imgRect.size.height },
                { width, height - (imgRect.origin.y + imgRect.size.height) }
            };
            CGContextFillRect(cgContext, bgBottom);
        }

    } else {
        CGRect cgBounds = {
            { 0, 0 },
            { width, height }
        };
        CGContextFillRect(cgContext, cgBounds);
    }

    CGColorRelease(bgColor);
    CGColorSpaceRelease(cSpace);
    CGContextRestoreGState(cgContext);

    return true; // This is handled
}
