// Name:    playuvApp.h
// Purpose: Main window of the YUV player application
// Author:  Giuseppe Baruffa

// Versioning
#define  PYUV_SHORTNAME         "PYUV"
#define  PYUV_VERSION           0
#define  PYUV_SUBVERSION        6
#define  PYUV_REVISION          1
#define  PYUV_BETA              "fork"
#define  PYUV_BTIME             wxT(__DATE__) wxT(", ") wxT(__TIME__)
#define  PYUV_WEBSITE           "http://dsplab.diei.unipg.it/pyuv_raw_video_sequence_player"
#define  PYUV_GITHUB            "https://github.com/tufei/pyuv-player"

// Standard dims for the frame
#define  PYUV_WIDTH             250
#define  PYUV_HEIGHT            200
#define  PYUV_DEPTH             8
#define  PYUV_SIGNED            false
#define  PYUV_MINWIDTH          150
#define  PYUV_MINHEIGHT         100
#define  PYUV_RATE              25.00
#define  PYUV_FORMAT            3
#define  PYUV_VERTBORD          2
#define  PYUV_HORIZBORD         2

// Other small things
#define  HELP_FILENAME          "doc.htb"

// Classes declaration

////////////////////////////////////
// Interprocess communications
////////////////////////////////////
#define  MAX_NO_SERVERS         20

// Server class, for listening to connection requests
class pyuvServer : public wxServer
{
public:
    pyuvServer(pyuvFrame *frame) : frame(frame) {};
    wxConnectionBase *OnAcceptConnection(const wxString& topic);

private:
    pyuvFrame *frame;
};

// Connection class, for use by both communicating instances
class pyuvConnection : public wxConnection
{
public:
    pyuvConnection(pyuvFrame *frame) : frame(frame) {};
    ~pyuvConnection() {};
    bool OnExecute(const wxString& topic, char *data, int WXUNUSED(size), wxIPCFormat WXUNUSED(format));

private:
    pyuvFrame *frame;
};

// Client class, to be used by subsequent instances in OnInit
class pyuvClient : public wxClient
{
public:
    pyuvClient() {};

    wxConnectionBase *OnMakeConnection() { return new pyuvConnection(frame); }

    void SetFrame(pyuvFrame *frame) { frame = frame; }

private:
    pyuvFrame *frame;
};

//////////////////////////////////////////////
// This is the application class declaration
//////////////////////////////////////////////
class pyuvApp : public wxApp
{
public:
    // Called on application startup
    virtual bool OnInit();
    virtual int OnExit();

    static int& GetServerNo(void) { return pyuvServno; }
    static int& GetMyMaster(void) { return pyuvMyMaster; }
    static bool& GetHaveClientFlag(void) { return pyuvIHaveASlave; }
    static bool (*GetMyClient(void))[MAX_NO_SERVERS] { return &pyuvMySlave; }

    static wxColour& GetTimeColour(void) { return pyuvTimeColour; }
    static wxColour& GetIdColour(void) { return pyuvIdColour; }
    static wxRect& GetTimeBack(void) { return pyuvTimeBack; }
    static wxRect& GetIdBack(void) { return pyuvIdBack; }

    static wxString& GetTopBaseTitle(void) { return pyuvTopbasetitle; }
    static wxString GetServerNoString(void);

    static wxColour (*GetClientColour(void))[MAX_NO_SERVERS] { return &pyuvSlaveColors; }

private:
    pyuvFrame *frame;

    static const wxCmdLineEntryDesc pyuvCommandLine[];

    // Timecode and identity boxes
    static wxColour pyuvTimeColour;
    static wxColour pyuvIdColour;
    static wxRect pyuvTimeBack;
    static wxRect pyuvIdBack;
    static wxString pyuvTopbasetitle;

    static wxColour pyuvSlaveColors[MAX_NO_SERVERS];

    // Synchronization
    static int pyuvServno;
    static int pyuvMyMaster;
    static bool pyuvIHaveASlave;
    static bool pyuvMySlave[MAX_NO_SERVERS];

    pyuvServer *m_server;
};

// Implements pyuvApp& GetApp()
wxDECLARE_APP(pyuvApp);

/////////////////////////////////////////
// Define a canvas for drawing onto
/////////////////////////////////////////
typedef wxPixelData<wxBitmap, wxNativePixelFormat> PixelData;

class pyuvCanvas : public wxPanel
{
public:
    // Constructor
    pyuvCanvas(pyuvFrame *parent);

    // A device context
    //wxClientDC cdc;

    // The two images used for rendering
    wxImage img; // basic canvas
    wxImage logoimg; // default logo displaied when no file is loaded
    wxBitmap *pyuvBmp; // the bitmap used for drawing
    PixelData *pyuvBmpdata; // raw bitmap data
    PixelData::Iterator *pyuvPixel;
    PixelData::Iterator *pyuvRowstart;

    // Some handlers
    void OnMotion(wxMouseEvent &event); // moving mouse on canvas
    void OnPaint(wxPaintEvent &event); // redraw the canvas
    void OnDrop(wxDropFilesEvent& event); // receive a dragged file

private:

    // This class handles events
    wxDECLARE_EVENT_TABLE();
};

