
#include "wx/wx.h"
#include <wx/listctrl.h>
#include <wx/clipbrd.h>

//#include "CMathParser.h"

#include "mathparser.h"


enum {
	Turnsticks_Quit = wxID_EXIT,
	Turnsticks_About = wxID_ABOUT
};

class myPane: public wxPanel
{
public:
	myPane(wxWindow *parent): wxPanel(parent, wxID_ANY)
	{
	
	}

private:
	wxTextCtrl *expression;
	wxStaticText *answer;
};

class MyFrame : public wxFrame
{
public:
	MyFrame(const wxString& title): wxFrame(NULL, wxID_ANY, title)
	{

		wxMenu *fileMenu = new wxMenu;

		wxMenu *helpMenu = new wxMenu;
		helpMenu->Append(Turnsticks_About, "&About\tF1", "Show about dialog");
		fileMenu->Append(Turnsticks_Quit, "E&xit\tAlt-X", "Quit this program");

		wxMenuBar *menuBar = new wxMenuBar();
		menuBar->Append(fileMenu, "&File");
		menuBar->Append(helpMenu, "&Help");

		SetMenuBar(menuBar);
		
		expression = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(250,-1), wxTE_PROCESS_ENTER);
		turnsize = new wxTextCtrl(this, wxID_ANY, "50", wxDefaultPosition,  wxDefaultSize, wxTE_PROCESS_ENTER);
		ticksize = new wxTextCtrl(this, wxID_ANY, "0.001", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
		answers = new wxListView(this, wxID_ANY, wxDefaultPosition, wxSize(250,400));
		answers->AppendColumn("answers:");
		answers->EnableAlternateRowColours();
		answers->SetColumnWidth(0, 200);
		
		expression->SetFocus();
		
		wxBoxSizer *h = new wxBoxSizer(wxHORIZONTAL);
		wxBoxSizer *v1 = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer *v2 = new wxBoxSizer(wxVERTICAL);
		
		int border=5;
		v1->Add(new wxStaticText(this, wxID_ANY, "expression:"),wxSizerFlags().Border(wxTOP|wxLEFT|wxRIGHT, border));
		v1->Add(expression, wxSizerFlags().Border(wxALL, border));
		v1->Add(answers, wxSizerFlags().Expand().Border(wxALL, border));
		h->Add(v1, wxSizerFlags().Border(wxALL, border));
		v2->Add(new wxStaticText(this, wxID_ANY, "turn size:"), wxSizerFlags().Border(wxTOP|wxLEFT|wxRIGHT, border));
		v2->Add(turnsize, wxSizerFlags().Border(wxALL, border));
		v2->Add(new wxStaticText(this, wxID_ANY, "tick size:"), wxSizerFlags().Border(wxTOP|wxLEFT|wxRIGHT, border));
		v2->Add(ticksize, wxSizerFlags().Border(wxALL, border));
		h->Add(v2, wxSizerFlags().Border(wxALL, border));
		SetSizer(h);
		

		CreateStatusBar();

		Bind(wxEVT_PAINT, &MyFrame::OnPaint, this);
		expression->Bind(wxEVT_TEXT_ENTER, &MyFrame::OnEval, this);
		answers->Bind(wxEVT_KEY_DOWN, &MyFrame::OnKey, this);
		Bind(wxEVT_ENTER_WINDOW, &MyFrame::ClearStatus, this);

	}
	
	void ClearStatus(wxMouseEvent& event)
	{
		SetStatusText("");
	}
	
	double roundoff(double n) 
	{ 
		return round(n*1000.0) / 1000.0;
	}
	
	bool has_alpha(std::string s)
	{
		for (std::string::iterator it = s.begin(); it != s.end(); ++it)
			if (isalpha(*it)) return true;
		return false;
	}
	
	void OnEval(wxCommandEvent &event)
	{
		std::string expr = expression->GetValue().ToStdString();
		
		if (has_alpha(expr)) {
			SetStatusText("Error: alpha characters not allowed");
			return;
		}
		
		//CMathParser mp;
		Parser mp;
		float result;
		if (!mp.parse(expression->GetValue().ToStdString(), result)) {
		//if (mp.Calculate(expression->GetValue().ToStdString().c_str(), &result) != CMathParser::ResultOk) {
			SetStatusText(wxString(mp.getError()));
		}
		else {
			double ticksz = atof(ticksize->GetValue().ToStdString().c_str());
			double turnsz = atof(turnsize->GetValue().ToStdString().c_str());
			double thous = roundoff(result / ticksz);
			double turns = floor(thous / (int) turnsz);
			double ticks = thous - turns*turnsz ;
			if (atof(expression->GetValue().ToStdString().c_str()) == result)
				answers->InsertItem(0,  wxString::Format("%s: (%g/%g)", expression->GetValue(), turns, ticks));
			else
				answers->InsertItem(0,  wxString::Format("%s: %0.4f (%g/%g)", expression->GetValue(), result, turns, ticks));
			SetStatusText("");
		}
	}
	
	void OnKey(wxKeyEvent& event)
	{
		int itemcount;
		long item;
		wxString items;
		//printf("keycode: %d\n", event.GetKeyCode()); fflush(stdout);
		switch ( event.GetKeyCode() )
        {
            case WXK_DELETE:  //delete selected answer(s)
				itemcount = answers->GetSelectedItemCount();
				for (int i = 1; i<=itemcount; i++)
					answers->DeleteItem(answers->GetFirstSelected());
				SetStatusText(wxString::Format("Deleted %d items",itemcount));
				break;
			case WXK_CONTROL_Z:  //undo most recent delete
			
				break;
			case 67: //Ctrl-C, copy selected answers
				if (event.ControlDown()) {
					itemcount = answers->GetSelectedItemCount();
					item = answers->GetFirstSelected();
					items.Append(wxString::Format("%s\n", answers->GetItemText(item)));
					for (int i = 1; i<itemcount; i++) {
						item = answers->GetNextSelected(item);
						items.Append(wxString::Format("%s\n", answers->GetItemText(item)));
					}
					if (wxTheClipboard->Open())
					{
						wxTheClipboard->SetData( new wxTextDataObject(items) );
						wxTheClipboard->Close();
						SetStatusText(wxString::Format("Copied %d items to clipboard", itemcount));
					}
					else
						SetStatusText(wxString::Format("Copy failed."));
				}
				break;
			case 88: //cut selected answers from list
				if (event.ControlDown()) {
					itemcount = answers->GetSelectedItemCount();
					for (int i = 1; i<=itemcount; i++) {
						item = answers->GetFirstSelected();
						items.Append(wxString::Format("%s\n", answers->GetItemText(item)));
						answers->DeleteItem(answers->GetFirstSelected());
					}
					if (wxTheClipboard->Open())
					{
						wxTheClipboard->SetData( new wxTextDataObject(items) );
						wxTheClipboard->Close();
						SetStatusText(wxString::Format("Cut %d items to clipboard", itemcount));
					}
					else
						SetStatusText(wxString::Format("Cut failed."));
				}
				break;
			case WXK_CONTROL_V: //paste expression from copied answer
			
				break;
		}
		
	}

	void OnPaint(wxPaintEvent& WXUNUSED(event))
	{
		wxPaintDC dc(this);
		//put dc.DrawWhatever() here...
	}
	
	void OnQuit(wxCommandEvent& WXUNUSED(event))
	{
		Close(true);
	}
	
	void OnAbout(wxCommandEvent& WXUNUSED(event))
	{
		wxMessageBox("turnsticks: Calculates handwheel turns and ticks for measurements.");
	}

private:
	wxTextCtrl *expression, *turnsize, *ticksize;
	wxListView *answers;

	wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(Turnsticks_Quit,  MyFrame::OnQuit)
    EVT_MENU(Turnsticks_About, MyFrame::OnAbout)
wxEND_EVENT_TABLE()


class MyApp : public wxApp
{
public:
	bool OnInit()
	{
		if ( !wxApp::OnInit() )
		return false;

		MyFrame *frame = new MyFrame("turnsticks");
		frame->Show(true);
		return true;
	}
};

wxIMPLEMENT_APP(MyApp);


