// LenetWDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LenetW.h"
#include "LenetWDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	
// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CLenetWDlg dialog
CLenetWDlg::CLenetWDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLenetWDlg::IDD, pParent)
	, m_epat_max(0)
	, m_epat_no(0)
	, m_eresult(0)
	, position_X(0)
	, position_Y(0)
	, m_eerror(0)
	, m_edit_class(0)
	, m_eradius(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_iPrevX = 0;
	m_iPrevY = 0;
}

void CLenetWDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT9, m_epat_max);
	DDX_Text(pDX, IDC_EDIT8, m_epat_no);
	DDX_Control(pDX, IDC_SLIDER1, m_slider);
	DDX_Control(pDX, IDC_BUTTON3, m_epat_prev);
	DDX_Control(pDX, IDC_BUTTON4, m_epat_next);
	DDX_Text(pDX, IDC_EDIT11, m_eresult);
	DDX_Control(pDX, IDC_BUTTON8, m_brun);
	DDX_Text(pDX, IDC_EDIT11, m_eresult);
	DDX_Control(pDX, IDC_TREE1, m_treelist);
	DDX_Text(pDX, IDC_EDIT12, m_eerror);
	DDX_Text(pDX, IDC_EDIT1, m_edit_class);
	DDX_Text(pDX, IDC_EDIT13, m_eradius);
}

BEGIN_MESSAGE_MAP(CLenetWDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnOpenNETButton)
	ON_BN_CLICKED(IDC_BUTTON2, OnOpenPATButton)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, OnNMReleasedcaptureSlider1)
	ON_BN_CLICKED(IDC_BUTTON3, OnPatternBackButton)
	ON_BN_CLICKED(IDC_BUTTON4, OnPatternForwardButton)
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON8, OnRUNButton)
	ON_WM_MOVE()
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_PAINT_BLACK, OnPaintBlack)
	ON_COMMAND(ID_PAINT_WHITE, OnPaintWhite)
	ON_COMMAND(ID_PAINT_2PIXELS, OnPaint2pixels)
	ON_COMMAND(ID_PAINT_4PIXELS, OnPaint4pixels)
	ON_COMMAND(ID_PAINT_6PIXELS, OnPaint6pixels)
	ON_COMMAND(ID_PAINT_8PIXELS, OnPaint8pixels)
	ON_COMMAND(ID_PAINT_10PIXELS, OnPaint10pixels)
	ON_COMMAND(ID_PAINT_CLEAR, OnPaintClear)
	ON_BN_CLICKED(IDC_BUTTON5, OnGoToButton)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// CLenetWDlg message handlers

BOOL CLenetWDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	//********************************************************
	//inicializacia parametrov

	zoom.Create(IDD_ZOOM,this);
	m_eresult = -1;
	m_eerror = -1.0;
	m_epat_no = 0;
	UpdateData(FALSE);

	p_INP = NULL;
	p_OUT = NULL;
	for (int i = 0 ; i < _MAX_CONVOLUTIONS ; i++) 
		p_C[i] = NULL;
	for (int i = 0 ; i < _MAX_SAMPLINGS ; i++) 
		p_S[i] = NULL;
	for (int i = 0 ; i < _MAX_CLASSICS ; i++) 
		p_F[i] = NULL;
	p_vstup = NULL;
	info = NULL;

	_CONVOLUTIONS = 0;			//uchovavaju skutocny pocet vrstiev v sieti
	_SAMPLINGS = 0;				//pocet konvolucii a vzorkovani sa musi rovnat !!!
	_CLASSICS = 0;
	
	brush_width = 4;
	brush_color = RGB(0,0,0);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLenetWDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CLenetWDlg::OnDestroy()
{
	WinHelp(0L, HELP_QUIT);
	CDialog::OnDestroy();
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLenetWDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	
		OnPaintClear();
		PrintStructure();				//vykreslenie struktury
		PrintFields();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLenetWDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//********************************************************
//nacitanie siete zo suboru 'Open NET file'
void CLenetWDlg::OnOpenNETButton()
{
	static char BASED_CODE patFilter[] = "Pattern Files (*.net)|*.net|All Files (*.*)|*.*||";
	CFileDialog x(TRUE, NULL, NULL, OFN_HIDEREADONLY, patFilter, NULL);
	
	x.DoModal();
	CString file_name, full_file_name;
	full_file_name = x.GetPathName();	
	file_name = x.GetFileName();

	if ( file_name != "" ) {
		
		delete[] p_INP;
		delete[] p_OUT;
		for (int i = 0 ; i < _MAX_CONVOLUTIONS ; i++) 
			delete[] p_C[i];
		for (int i = 0 ; i < _MAX_SAMPLINGS ; i++) 
			delete[] p_S[i];
		for (int i = 0 ; i < _MAX_CLASSICS ; i++) 
			delete[] p_F[i];

		initialize_net(full_file_name, false);

		OnPaintClear();

		m_brun.EnableWindow(TRUE);
		CString caption = "LenetW - " + file_name;
		CLenetWDlg::SetWindowText(caption);
		UpdateData(FALSE);
		PrintStructure();
		PrintFields();
	}
}
//********************************************************
//otvorenie testovacej mnoziny 'Open PATTERN file'
void CLenetWDlg::OnOpenPATButton()
{
	static char BASED_CODE patFilter[] = "Image Files (*.images)|*.images|All Files (*.*)|*.*||";
	
	CFileDialog x(TRUE, NULL, NULL, OFN_HIDEREADONLY, patFilter, NULL);
	
	x.DoModal();
	CString file_name, full_file_name, full_file_name_membership;
	full_file_name = full_file_name_membership = x.GetPathName();	
	file_name = x.GetFileName();
	
	int ii = full_file_name_membership.Find(".images");
	full_file_name_membership.Delete(ii, 7);
	full_file_name_membership += ".labels"; 	

	if (file_name != "") {
		delete p_vstup;
		
		p_vstup = new read_file();
		p_vstup->open_file(full_file_name, full_file_name_membership);
		p_vstup->read_header();
		p_vstup->read_patterns();
	
		m_epat_max = p_vstup->patterns;
		m_epat_no = 1;
		
		m_edit_class = p_vstup->pattern[0].membership;

		long step = m_epat_max / 100 ;
		if (step == 0) step = 1;

		m_slider.EnableWindow(TRUE);
		m_slider.SetRange(1,m_epat_max);
		m_slider.SetPos(1);
		m_slider.SetPageSize(step);
	
		m_epat_prev.EnableWindow(TRUE);
		m_epat_next.EnableWindow(TRUE);
		PrintInput();
	}
	UpdateData(FALSE);
}

//******************************************************
//vykreslenie vzoru cast 'Input'
void CLenetWDlg::PrintInput(void)
{
	CClientDC dc(this);
	int x_scale, y_scale;
	dc.SetViewportOrg(20,30);
		if (p_INP != NULL) {
			dc.Rectangle(-1,-1,2*p_INP->sizeM+1,2*p_INP->sizeN+1);	//bude to zvacsene 2x
			x_scale = (p_INP->sizeM - p_vstup->x_size) / 2;
			y_scale = (p_INP->sizeN - p_vstup->y_size) / 2;
		}
		else { 
			dc.Rectangle(-1,-1,2*p_vstup->x_size+1,2*p_vstup->y_size+1);	//bude to zvacsene 2x
			x_scale = 0;
			y_scale = 0;
		}
		for (int i = 0 ; i < p_vstup->x_size*2  ; i++) {
			for (int j = 0 ; j < p_vstup->y_size*2 ; j++) {
				int pix = 255 - p_vstup->pattern[m_epat_no-1].pixel[i/2][j/2];	
				//vymenene i,j koli spravnemu zobr.
				//dc.SetPixelV(j + y_scale*2, i + x_scale*2, RGB(pix,pix,pix));		
				dc.SetPixelV(i + x_scale*2, j + y_scale*2, RGB(pix,pix,pix));		
			}
		}
		dc.SetViewportOrg(305,230);
		dc.Rectangle(-1, -1, 100*4+1, 1*4+1);	//zvacsenie 4x
		
}

//********************************************************
//pohyb mysou po slideri
void CLenetWDlg::OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	m_epat_no = m_slider.GetPos();
	m_edit_class = p_vstup->pattern[m_epat_no-1].membership;
	PrintInput();
	UpdateData(FALSE);
}

//********************************************************
//posun o jeden pattern dozadu '<'
void CLenetWDlg::OnPatternBackButton()
{
	long temp = m_slider.GetPos();
	if (temp != 1) {
		m_slider.SetPos(temp-1);
		m_epat_no = temp-1;
		m_edit_class = p_vstup->pattern[m_epat_no-1].membership;
	}
	PrintInput();
	UpdateData(FALSE);
}

//********************************************************
//posun o jeden pattern dopredu '>'
void CLenetWDlg::OnPatternForwardButton()
{
	long temp = m_slider.GetPos();
	if (temp != m_epat_max) {
		m_slider.SetPos(temp+1);
		m_epat_no = temp+1;
		m_edit_class = p_vstup->pattern[m_epat_no-1].membership;
	}
	PrintInput();
	UpdateData(FALSE);
}

//******************************************************
//obsluha kurzora pre okno
BOOL CLenetWDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	CWinApp* pApp = AfxGetApp();
	CPoint  p_Cursor, p_Position;	
	WINDOWPLACEMENT wp;

	GetCursorPos( &p_Cursor );	//poloha kurzora
	GetWindowPlacement(&wp);	//poloha okna programu

	p_Position.x = p_Cursor.x - wp.rcNormalPosition.left - 2;
	p_Position.y = p_Cursor.y - wp.rcNormalPosition.top - 22;
	if (p_INP != NULL) {
		if ( p_Position.x > 20 && 
		 p_Position.x < 20+p_INP->sizeM*2 &&
		 p_Position.y > 30 &&
		 p_Position.y < 30+p_INP->sizeN*2 ) {
		HICON hIconBang = pApp->LoadStandardCursor( IDC_CROSS );
		SetCursor( hIconBang );
		}
		else {
		HICON hIconBang = pApp->LoadStandardCursor( IDC_ARROW );
		SetCursor( hIconBang );
		}
	}
	else {
		HICON hIconBang = pApp->LoadStandardCursor( IDC_ARROW );
		SetCursor( hIconBang );
	}
	
	return TRUE;
	//return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

//******************************************************** 
//pohyb mysou po okne programu + kreslenie v orezanej casti L-tlacidlom
void CLenetWDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CClientDC dc(this);

	if (p_INP != NULL) {
	if ( point.x > 19+brush_width/2 && 
		 point.x < 20+p_INP->sizeM*2+1-brush_width/2 &&
		 point.y > 29+brush_width/2 &&
		 point.y < 30+p_INP->sizeN*2+1-brush_width/2) {
		CPen myPen(PS_SOLID, brush_width, brush_color);
		CPen* oldPen = dc.SelectObject( &myPen );
	
		if ((nFlags & MK_LBUTTON) == MK_LBUTTON) {
			dc.MoveTo(m_iPrevX, m_iPrevY);
			dc.LineTo(point.x, point.y);
		}
		m_iPrevX = point.x;
		m_iPrevY = point.y;
	}
	}
	if (nFlags ==  MK_LBUTTON)
		OnLButtonDown(nFlags, point);

	CDialog::OnMouseMove(nFlags, point);
}

//******************************************************** 
//vykreslenie struktury siete Lenet
void CLenetWDlg::PrintStructure(void)
{
	CClientDC dc(this);
	int i, j, k, m, n, p_x, p_y;
	int temp, temp2;

	if ( info != NULL) {
		//nastavenie suradnicoveho systemu
		dc.SetViewportOrg(299,29);
		dc.Rectangle(-5,-5,590,150);
		//vykreslenie planov pre cely Lenet
		p_x = 0;
		p_y = 0;
		int convol = 0;
		int sampl = 0;
		int classic = 0;
		float value;
		for (int a = 0 ; a < _CONVOLUTIONS+_SAMPLINGS+_CLASSICS ; a++) {
			switch (info[a]) {
				case 'c':				
				p_x = 0;
				for (i = 0 ; i < p_C[convol]->this_planes ; i++) {
					dc.Rectangle(p_x, p_y, p_x + p_C[convol]->sizeM + 2, p_y + p_C[convol]->sizeN + 2);
					for (m = 1 ; m <= p_C[convol]->sizeM ; m++) {
						for (n = 1 ; n <= p_C[convol]->sizeN ; n++) {
							
							if (p_C[convol]->plane[i].x[m-1][n-1] > 1.7)
								temp = 0;
							else if (p_C[convol]->plane[i].x[m-1][n-1] < -1.7)
								temp = 255;
							else
								temp = 255 - (int)((p_C[convol]->plane[i].x[m-1][n-1] + 1.7)*75);
							dc.SetPixelV(p_x + m, p_y + n, RGB(temp,temp,temp));
							
						}
					}
					p_x = p_x + p_C[convol]->sizeM + 5;
				}
				p_y += p_C[convol]->sizeN + 3; 
				convol++;
				break;
				case 's':
				p_x = 0;
				for (i = 0 ; i < p_S[sampl]->this_planes ; i++) {
					dc.Rectangle(p_x, p_y, p_x + p_S[sampl]->sizeM + 2, p_y + p_S[sampl]->sizeN + 2);
					for (m = 1 ; m <= p_S[sampl]->sizeM ; m++) {
						for (n = 1 ; n <= p_S[sampl]->sizeN ; n++) {
							if (p_S[sampl]->plane[i].x[m-1][n-1] > 1.7)
								temp = 0;
							else if (p_S[sampl]->plane[i].x[m-1][n-1] < -1.7)
								temp = 255;
							else
								temp = 255 - (int)((p_S[sampl]->plane[i].x[m-1][n-1] + 1.7)*75);
							dc.SetPixelV(p_x + m, p_y + n, RGB(temp,temp,temp));
						}
					}
					p_x = p_x + p_S[sampl]->sizeM + 5;
				}
				p_y += p_S[sampl]->sizeN + 3;
				sampl++;
				break;
				case 'f':
				if (classic == _CLASSICS-1) {
					//nastavenie do Output okna
					
					//chromozom
					dc.SetViewportOrg(305,220);
					dc.Rectangle(-1, -1, 100*4+1, 1*4+1);	//zvacsenie 4x
					k = 0;
					for (m = 0 ; m < 1*4 ; m = m + 4) {
						for (n = 0 ; n < 100*4 ; n = n + 4) {
							if (p_F[classic]->plane[k].x > 1.7)
								temp = 0;
							else if (p_F[classic]->plane[k].x < -1.7)
								temp = 255;
							else 
								temp = 255 - (int)((p_F[classic]->plane[k].x + 1.7)*75);
							for (i = 0 ; i < 4 ; i++) {
								for (j = 0 ; j < 4 ; j++) {
									dc.SetPixelV(n+i, m+j, RGB(temp,temp,temp));
								}
							}
							k++;
						}
					}	
					
					//chromozom
					dc.SetViewportOrg(305,230);
					dc.Rectangle(-1, -1, 100*4+1, 1*4+1);	//zvacsenie 4x
					k = 0;
					for (m = 0 ; m < 1*4 ; m = m + 4) {
						for (n = 0 ; n < 100*4 ; n = n + 4) {
							if (p_F[classic]->plane[k].x > 1.0) {
								temp = 0;
								temp2 = 255 - (int)((p_F[classic]->plane[k].x - 1.0)*364);
							}
							else if (p_F[classic]->plane[k].x < -1.0) {
								temp = 255;
								temp2 = 255 - (int)((fabs(p_F[classic]->plane[k].x) - 1.0)*364);
							}
							else {
								temp = 255 - (int)((p_F[classic]->plane[k].x + 1.0)*127.5);
								temp2 = temp;
							}
							for (i = 0 ; i < 4 ; i++) {
								for (j = 0 ; j < 4 ; j++) {
									dc.SetPixelV(n+i, m+j, RGB(temp2,temp,temp));
								}
							}
							k++;
						}
					}

					
					//cisla
					dc.SetViewportOrg(225,36);
					dc.Rectangle(-1, -1, 7*4+1, 12*4+1);	//zvacsenie 4x
					k = 0;
					for (m = 0 ; m < 12*4 ; m = m + 4) {
						for (n = 0 ; n < 7*4 ; n = n + 4) {
							if (p_F[classic]->plane[k].x > 1.0)
								temp = 0;
							else if (p_F[classic]->plane[k].x < -1.0)
								temp = 255;
							else 
								temp = 255 - (int)((p_F[classic]->plane[k].x + 1.0)*127.5);
							for (i = 0 ; i < 4 ; i++) {
								for (j = 0 ; j < 4 ; j++) {
									dc.SetPixelV(n+i, m+j, RGB(temp,temp,temp));
								}
							}
							k++;
						}
					}	
					
				}
				else {
					p_x = 0;
					dc.Rectangle(p_x, p_y, p_x + p_F[classic]->size*p_F[classic]->this_planes + 2 , p_y + p_F[classic]->size + 3);
					for (m = 1 ; m <= p_F[classic]->this_planes ; m++) {
						if (p_F[classic]->plane[m-1].x > 1.7)
							temp = 0;
						else if (p_F[classic]->plane[m-1].x < -1.7)
							temp = 255;
						else
							temp = 255 - (int)((p_F[classic]->plane[m-1].x + 1.7)*75);
						dc.SetPixelV(p_x + m, p_y + 1, RGB(temp,temp,temp));
						dc.SetPixelV(p_x + m, p_y + 2, RGB(temp,temp,temp));
					}
					p_y += p_F[classic]->size + 4;
				}
				classic++;
				break;
			}
		}
	}		
}

//******************************************************** 
//prechod sietou 'RUN'
void CLenetWDlg::OnRUNButton()
{
	CClientDC dc(this);
	COLORREF col;
	int i, j, input;
	//int pole[32][32];

	//priradenie nakresleneho vstupu na siet
	dc.SetViewportOrg(20,30);
	for( i = 0 ; i < p_INP->sizeM ; i++) {
		for ( j = 0 ; j < p_INP->sizeN ; j++) {
						
			col = dc.GetPixel(i*2,j*2);
			input = 255 - (int)(GetRValue(col)+GetGValue(col)+GetBValue(col))/3;
			col = dc.GetPixel(i*2+1,j*2);
			input = input + 255 - (int)(GetRValue(col)+GetGValue(col)+GetBValue(col))/3;
			col = dc.GetPixel(i*2,j*2+1);
			input = input + 255 - (int)(GetRValue(col)+GetGValue(col)+GetBValue(col))/3;
			col = dc.GetPixel(i*2+1,j*2+1);
			input = input + 255 - (int)(GetRValue(col)+GetGValue(col)+GetBValue(col))/3;
			input = input / 4;
			//pole[i][j] = input;
			p_INP->x[i][j] = (float)input/200.0 - 0.1;
			
		}
	}
	
	//prechod sietou
	FeedForward();
	//zobrazenie stavu siete
	PrintStructure();
	//m_eresult = RbfCount();
	m_eresult = GetOutput();
	m_eerror = get_output_activation();
	UpdateData(FALSE);

}

//********************************************************    
//funkcia na inicializaciu novej siete pre ucenie
void CLenetWDlg::initialize_net(CString name, bool initialize) 
{
	FILE *def;
	char temp_string[100];						//pomocne premenne
	char buffer[100];
	int **temp_field;							//pomocne premenne
	int t1, t2, t11, t12, t21, t22, t3, t4, counter, treecounter;	//pomocne premenne
	CString text, textM, textN;

	if( !(def=fopen(name,"rt"))) {
		printf("\nError in opening file %s !", name);
		exit(0);
	}
	else {
		m_treelist.DeleteAllItems();
		HTREEITEM hNetwork = m_treelist.InsertItem("Network");
		HTREEITEM hLayers = m_treelist.InsertItem("Layers");
		HTREEITEM *hLayerDetail;
		//nastavenie topologie
		fscanf(def, "%s", &temp_string);
		fscanf(def, "\n%d %d %d", &_CONVOLUTIONS, &_SAMPLINGS, &_CLASSICS);
			info = new char[_CONVOLUTIONS+_SAMPLINGS+_CLASSICS+1];
			counter = 0;
			treecounter = 0;
			_itoa(_CONVOLUTIONS, buffer, 10);
			strcat(buffer, " convolution layer(s)");
			m_treelist.InsertItem(TVIF_TEXT, buffer, 0, 0, 0, 0, 0, hNetwork, NULL);
			_itoa(_CONVOLUTIONS, buffer, 10);
			strcat(buffer, " subsampling layer(s)");
			m_treelist.InsertItem(TVIF_TEXT, buffer, 0, 0, 0, 0, 0, hNetwork, NULL);
			_itoa(_CONVOLUTIONS, buffer, 10);
			strcat(buffer, " full-connected layer(s)");
			m_treelist.InsertItem(TVIF_TEXT, buffer, 0, 0, 0, 0, 0, hNetwork, NULL);
			_itoa(1+_CONVOLUTIONS+_SAMPLINGS+_CLASSICS+1, buffer, 10);
			strcat(buffer, " layer(s) total");
			m_treelist.InsertItem(TVIF_TEXT, buffer, 0, 0, 0, 0, 0, hNetwork, NULL);
		hLayerDetail = new HTREEITEM[1+_CONVOLUTIONS+_SAMPLINGS+_CLASSICS+1];
		if (_CONVOLUTIONS != _SAMPLINGS)
			exit(0);
		fscanf(def, "%s", &temp_string);
		fscanf(def, "\n%d %d", &t1, &t2);	
			text = _itoa(treecounter, buffer, 10);
			hLayerDetail[treecounter] = m_treelist.InsertItem(TVIF_TEXT, text + " - input", 0, 0, 0, 0, 0, hLayers, NULL);
			textM = _itoa(t1, buffer, 10);
			textN = _itoa(t2, buffer, 10);
			m_treelist.InsertItem(TVIF_TEXT, "size: " + textM + "x" + textN + "px", 0, 0, 0, 0, 0, hLayerDetail[treecounter], NULL);
			treecounter++;
			p_INP = new input_layer(t1, t2);
			t4 = 1;
		for (int a = 0 ; a < _CONVOLUTIONS ; a++) {
			fscanf(def, "\n%s", &temp_string);
			fscanf(def, "\n%d %d %d %d %d", &t3, &t11, &t12, &t21, &t22);
				text = _itoa(treecounter, buffer, 10);
				hLayerDetail[treecounter] = m_treelist.InsertItem(TVIF_TEXT, text + " - convolutional", 0, 0, 0, 0, 0, hLayers, NULL);
				textM = _itoa(t11, buffer, 10);
				textN = _itoa(t12, buffer, 10);
				m_treelist.InsertItem(TVIF_TEXT, "size: " + textM + "x" + textN + "px", 0, 0, 0, 0, 0, hLayerDetail[treecounter], NULL);
				textM = _itoa(t21, buffer, 10);
				textN = _itoa(t22, buffer, 10);
				m_treelist.InsertItem(TVIF_TEXT, "receptive: " + textM + "x" + textN + "px", 0, 0, 0, 0, 0, hLayerDetail[treecounter], NULL);
				text = _itoa(t3, buffer, 10);
				m_treelist.InsertItem(TVIF_TEXT, "planes: " + text, 0, 0, 0, 0, 0, hLayerDetail[treecounter], NULL);
				treecounter++;
			fscanf(def, "\n%s", &temp_string);
				temp_field = new int*[t3];				
				for (int i = 0 ; i < t3 ; i++) 
					temp_field[i] = new int[t4];
				for (int i = 0 ; i < t3 ; i++) {
					for (int j = 0 ; j < t4 ; j++) {
						fscanf(def, "%d", &temp_field[i][j]);
					}
				}
				p_C[a] = new convol_layer(t11, t12, t21, t22, t3, t4, temp_field);          // tu sa vytvori convol vrstva
				
				info[counter] = 'c';
				counter++;
				for (int i = 0 ; i < t3 ; i++)
					delete[] temp_field[i];
				delete[] temp_field;
			fscanf(def, "\n%s", &temp_string);
			fscanf(def, "\n%d %d %d %d %d", &t3, &t11, &t12, &t21, &t22);
				text = _itoa(treecounter, buffer, 10);
				hLayerDetail[treecounter] = m_treelist.InsertItem(TVIF_TEXT, text + " - subsampling", 0, 0, 0, 0, 0, hLayers, NULL);
				textM = _itoa(t11, buffer, 10);
				textN = _itoa(t12, buffer, 10);
				m_treelist.InsertItem(TVIF_TEXT, "size: " + textM + "x" + textN + "px", 0, 0, 0, 0, 0, hLayerDetail[treecounter], NULL);
				textM = _itoa(t21, buffer, 10);
				textN = _itoa(t22, buffer, 10);
				m_treelist.InsertItem(TVIF_TEXT, "receptive: " + textM + "x" + textN + "px", 0, 0, 0, 0, 0, hLayerDetail[treecounter], NULL);
				text = _itoa(t3, buffer, 10);
				m_treelist.InsertItem(TVIF_TEXT, "planes: " + text, 0, 0, 0, 0, 0, hLayerDetail[treecounter], NULL);
				treecounter++;
				p_S[a] = new sampl_layer(t11, t12, t21, t22, t3);			// tu sa vytvori sample vrstva
				info[counter] = 's';
				counter++;
				t4 = t3;
		}
		for (int a = 0 ; a < _CLASSICS ; a++) {
			fscanf(def, "\n%s", &temp_string);
			fscanf(def, "\n%d %d %d", &t3, &t1, &t2);
				text = _itoa(treecounter, buffer, 10);
				hLayerDetail[treecounter] = m_treelist.InsertItem(TVIF_TEXT, text + " - full connected", 0, 0, 0, 0, 0, hLayers, NULL);
				text = _itoa(t1, buffer, 10);
				m_treelist.InsertItem(TVIF_TEXT, "size: " + text + "x" + text + "px", 0, 0, 0, 0, 0, hLayerDetail[treecounter], NULL);
				text = _itoa(t2, buffer, 10);
				m_treelist.InsertItem(TVIF_TEXT, "connections: " + text, 0, 0, 0, 0, 0, hLayerDetail[treecounter], NULL);
				text = _itoa(t3, buffer, 10);
				m_treelist.InsertItem(TVIF_TEXT, "planes: " + text, 0, 0, 0, 0, 0, hLayerDetail[treecounter], NULL);
				treecounter++;
				p_F[a] = new classic_layer(t1,t2,t3);						// tu sa vytvori classic vrstva 1 
				info[counter] = 'f';
				counter++;
		}
		fscanf(def, "\n%s", &temp_string);
		fscanf(def, "\n%d %d %d", &t3, &t1, &t2);
			text = _itoa(treecounter, buffer, 10);
			hLayerDetail[treecounter] = m_treelist.InsertItem(TVIF_TEXT, text + " - output", 0, 0, 0, 0, 0, hLayers, NULL);
			text = _itoa(t1, buffer, 10);
			m_treelist.InsertItem(TVIF_TEXT, "size: " + text + "x" + text + "px", 0, 0, 0, 0, 0, hLayerDetail[treecounter], NULL);
			text = _itoa(t2, buffer, 10);
			m_treelist.InsertItem(TVIF_TEXT, "connections: " + text, 0, 0, 0, 0, 0, hLayerDetail[treecounter], NULL);
			text = _itoa(t3, buffer, 10);
			m_treelist.InsertItem(TVIF_TEXT, "planes: " + text, 0, 0, 0, 0, 0, hLayerDetail[treecounter], NULL);
			treecounter++;
			p_OUT = new classic_layer (t1,t2,t3);							// tu sa vytvori classic vrstva 2
			info[counter] = 'o';
			counter++;

		//learn++ kompatibilita
		int temp_int;
		fscanf(def, "\n%s", &temp_string);
		fscanf(def, "\n");
		for (int a = 0 ; a < t3 ; a++) {
			fscanf(def, "%d", &temp_int);
			p_OUT->plane[a].membership = temp_int;
		}
		
		double temp_float;
		fscanf(def, "\n%s", &temp_string);
		fscanf(def, "%d", &temp_int);
		fscanf(def, "%lf", &temp_float);
		fscanf(def, "%lf", &temp_float);
		fscanf(def, "%lf", &temp_float);
		fscanf(def, "%lf", &temp_float);
		fscanf(def, "%lf", &temp_float);
		fscanf(def, "%lf", &temp_float);

		//ak je initialize = false, tak sa vahy nastavia podla suboru
		if (initialize == false) {
			int convol = 0;
			int sampl = 0;
			int classic = 0;
			float value;
			for (int a = 0 ; a < _CONVOLUTIONS+_SAMPLINGS+_CLASSICS+1 ; a++) {
				switch (info[a]) {
					case 'c':
					if (convol > _CONVOLUTIONS)
						exit(0);
					fscanf(def, "\n%s", &temp_string);
					for (int k = 0 ; k < p_C[convol]->this_planes ; k++) {
						fscanf(def, "%f", &value);	//bias
						p_C[convol]->plane[k].bias = value;
						int max;
						if (convol == 0)
							max = 1;	//bereme zo vstupu
						else
							max = p_S[sampl-1]->this_planes;	//z predch. vrstvy
						for (int h = 0 ; h < max ; h++) {
							for (int i = 0 ; i < p_C[convol]->receptiveM ; i++) {
								for (int j = 0 ; j < p_C[convol]->receptiveN ; j++) {
									fscanf(def, "%f", &value);
									p_C[convol]->plane[k].w[h][i][j] = value;
								}
							}
						}
					}
					convol++;
					break;
					case 's':
					if (sampl > _SAMPLINGS)
						exit(0);
					fscanf(def, "\n%s", &temp_string);
					for (int k = 0 ; k < p_S[sampl]->this_planes ; k++) {
						fscanf(def, "%f", &value);	//bias
						p_S[sampl]->plane[k].bias = value;
						fscanf(def, "%f", &value);
						p_S[sampl]->plane[k].w = value;
					}
					sampl++;
					break;
					case 'f':
					if (classic > _CLASSICS)
						exit(0);
					fscanf(def, "\n%s", &temp_string);
					for (int k = 0 ; k < p_F[classic]->this_planes ; k++) {
						fscanf(def, "%f", &value);	//bias
						p_F[classic]->plane[k].bias = value;
						for (int h = 0 ; h < p_F[classic]->connections ; h++) {
							fscanf(def, "%f", &value);
							p_F[classic]->plane[k].w[h] = value;
						}
					}
					classic++;
					break;
					case 'o':
					fscanf(def, "\n%s", &temp_string);
					for (int k = 0 ; k < p_OUT->this_planes ; k++) {
						fscanf(def, "%f", &value);	//bias
						p_OUT->plane[k].bias = value;
						for (int h = 0 ; h < p_OUT->connections ; h++) {
							fscanf(def, "%f", &value);
							p_OUT->plane[k].w[h] = value;
						}
					}
					break;
					
				}
			}
		}
	}
}

//*******************************************************
//prechod sietou
void CLenetWDlg::FeedForward(void)
{
	int k,i,j,m,n,h;
	int convol = 0;
	int sampl = 0;
	int classic = 0;
	float value;
	for (int a = 0 ; a < _CONVOLUTIONS+_SAMPLINGS+_CLASSICS ; a++) {
		switch (info[a]) {
			case 'c':
			for (k = 0 ; k < p_C[convol]->this_planes ; k++) {
				for (i = 0 ; i < p_C[convol]->sizeM ; i++) {
					for (j = 0 ; j < p_C[convol]->sizeN ; j++) {
						//vypocet hodnoty pre neuron vo vrstve C3
						float value = 0;
						//plany su pospajane podla connection_field
						for (h = 0 ; h < p_C[convol]->prev_planes ; h++) {
							int xx = 0;
							int yy = 0;
							for (m = i ; m < i + p_C[convol]->receptiveM /*- 1*/ ; m++) {
								for (n = j ; n < j + p_C[convol]->receptiveN /*- 1*/ ; n++) {
									if (convol == 0)
										value += p_INP->x[m][n]*p_C[convol]->plane[k].w[h][xx][yy];
									else
										value += p_S[sampl-1]->plane[h].x[m][n]*p_C[convol]->plane[k].w[h][xx][yy];
									yy++;
								}
								xx++;
								yy = 0;
							}
						}
						value += p_C[convol]->plane[k].bias;//*(-1);
						p_C[convol]->plane[k].x[i][j] = squash_f(value);
					}
				}
			}
			convol++;
			break;
			case 's':
			for (k = 0 ; k < p_S[sampl]->this_planes ; k++) {
				for (i = 0 ; i < p_S[sampl]->sizeM ; i++) {
					for (j = 0 ; j < p_S[sampl]->sizeN ; j++) {
						//vypocet hodnoty pre neuron vo vrstve S2
						float value = 0;
						int recM = p_S[sampl]->receptiveM;
						int recN = p_S[sampl]->receptiveN;
						for (m = i*recM ; m < i*recM + recM ; m++) {    // neviem ci su spravne indexy M N
							for (n = j*recN ; n < j*recN + recN ; n++) {
								value += p_C[convol-1]->plane[k].x[m][n];
							}
						}
						value *= p_S[sampl]->plane[k].w;
						value += p_S[sampl]->plane[k].bias;//*(-1);
						p_S[sampl]->plane[k].x[i][j] = squash_f(value);
					}
				}
			}
			sampl++;
			break;
			case 'f':
			for (k = 0 ; k < p_F[classic]->this_planes ; k++) {
				float value = 0;
				int temp = 0;
				if (classic == 0) {
					for (h = 0 ; h < p_S[sampl-1]->this_planes ; h++) {
						for (i = 0 ; i < p_S[sampl-1]->sizeM ; i++) {
							for (j = 0 ; j < p_S[sampl-1]->sizeN ; j++) {
								value += p_S[sampl-1]->plane[h].x[i][j]*p_F[classic]->plane[k].w[temp];
								temp++;
							}
						}
					}
				}
				else {
					for (h = 0 ; h < p_F[classic-1]->this_planes ; h++) {
						value += p_F[classic-1]->plane[h].x*p_F[classic]->plane[k].w[temp];
						temp++;
					}
				}
				value += p_F[classic]->plane[k].bias;//*(-1);
				p_F[classic]->plane[k].x = squash_f(value);
			}
			classic++;
			break;
		}
	}
}

//********************************************************
//prechodova funkcia
float CLenetWDlg::squash_f(float value)
{
	return (1.7159*tanh((2.0/3.0)*value));
	
}

//********************************************************
//prechod RBF casti siete
int CLenetWDlg::RbfCount(void)
{
	int temp = 0;
	//_CLASSIC-1 je index predposlednej vrstvy 
	for (int i = 0 ; i < p_OUT->this_planes ; i++) {
		float value = 0.0;
		for (int j = 0 ; j < p_F[_CLASSICS-1]->this_planes ; j++) {
			value += (p_F[_CLASSICS-1]->plane[j].x-p_OUT->plane[i].w[j])*(p_F[_CLASSICS-1]->plane[j].x-p_OUT->plane[i].w[j]);
		}
		p_OUT->plane[i].x = value;
	}
	float min = p_OUT->plane[0].x;
	for (int i = 1 ; i < p_OUT->this_planes ; i++) {
		if ( min > p_OUT->plane[i].x) {
			temp = i;
			min = p_OUT->plane[i].x;
		}
	}
	return temp;
}

//********************************************************
//pohyb hlavneho dialogu
void CLenetWDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	position_X = x;
	position_Y = y;
}

//********************************************************
//kontextove menu
void CLenetWDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu *m_lMenu;     

	m_lMenu = new CMenu;
	m_lMenu->LoadMenu(IDR_PAINTMENU);
	m_lMenu = m_lMenu->GetSubMenu(0);
	m_lMenu->TrackPopupMenu(TPM_CENTERALIGN + TPM_LEFTBUTTON, point.x, point.y, this);
}

//********************************************************
//pustenie laveho tlacidla mysi
void CLenetWDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
    CDialog::OnRButtonUp(nFlags, point);
	zoom.ShowWindow(SW_HIDE);
}

//********************************************************
//stlacenie laveho tlacidla mysi
void CLenetWDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CClientDC dc(this);
	//zoom pole o rozmere 30x30 pixelov
	COLORREF zoom_field[30][30];
	point.x = point.x - 14;
	point.y = point.y - 14;
	//ohranicenie
	if ( point.x > 293 && point.x < 860 && point.y > 23 && point.y < 150 
		 ||
		 point.x > 293 && point.x < 860 && point.y > 259 && point.y < 381 ) {
		for (int i = 0 ; i < 30 ; i++) {
			for (int j = 0 ; j < 30 ; j++) {
				zoom_field[i][j] = dc.GetPixel(point.x+i, point.y+j);
			}
		}
	
		zoom.ShowWindow(SW_RESTORE);
		zoom.set_position(position_X, position_Y);
		zoom.zoom_print(zoom_field);
	}
	/*
	else if ( point.x < 283 || point.x > 674 || point.y < 13 || point.y > 156 ) {
		zoom.ShowWindow(SW_HIDE);
	}
	*/
	CDialog::OnRButtonDown(nFlags, point);
}

//********************************************************
//nasleduju funkcie pop-up-menu
void CLenetWDlg::OnPaintBlack()
{
	brush_color = RGB(0,0,0);
}
void CLenetWDlg::OnPaintWhite()
{
	brush_color = RGB(255,255,255);
}
void CLenetWDlg::OnPaint2pixels()
{
	brush_width = 2;
}
void CLenetWDlg::OnPaint4pixels()
{
	brush_width = 4;
}
void CLenetWDlg::OnPaint6pixels()
{
	brush_width = 6;
}
void CLenetWDlg::OnPaint8pixels()
{
	brush_width = 8;
}
void CLenetWDlg::OnPaint10pixels()
{
	brush_width = 10;
}
void CLenetWDlg::OnPaintClear()
{
	CClientDC dc(this);
	dc.SetViewportOrg(0,0);
	if (p_INP != NULL)
		dc.Rectangle(19,29,20+p_INP->sizeM*2+1,30+p_INP->sizeN*2+1);
}

//********************************************************
//nastavenie na cislo vzorky
void CLenetWDlg::OnGoToButton()
{
	if (p_vstup != NULL) {
		UpdateData(true);
		if (m_epat_no < 1) {
			m_epat_no = 1;
		}
		if (m_epat_no > p_vstup->patterns) {
			m_epat_no = p_vstup->patterns;
		}

		m_slider.SetPos(m_epat_no);
		PrintInput();
		UpdateData(false);
	}
}

//********************************************************
//posuvanie sa po vzorkach
BOOL CLenetWDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	long temp = m_slider.GetPos();
	//pohyb po 1 vzorke dopredu a spat
	if (zDelta >= 120) {
		if (temp != m_epat_max) {
			m_slider.SetPos(temp+1);
			m_epat_no = temp+1;
		}
	}
	if (zDelta <= -120) {	
		if (temp != 1) {
			m_slider.SetPos(temp-1);
			m_epat_no = temp-1;
		}
	}
	//pohyb po 10 vzorkach dopredu a spat
	if (zDelta >= 120 && nFlags == MK_SHIFT) {
		if (temp < m_epat_max-10 ) {
			m_slider.SetPos(temp+10);
			m_epat_no = temp+10;
		}
	}
	if (zDelta <= -120 && nFlags == MK_SHIFT) {	
		if (temp > 1+10) {
			m_slider.SetPos(temp-10);
			m_epat_no = temp-10;
		}
	}
	//pohyb po 100 vzorkach dopredu a spat
	if (zDelta >= 120 && nFlags == MK_CONTROL) {
		if (temp < m_epat_max-100 ) {
			m_slider.SetPos(temp+100);
			m_epat_no = temp+100;
		}
	}
	if (zDelta <= -120 && nFlags == MK_CONTROL) {	
		if (temp > 1+100) {
			m_slider.SetPos(temp-100);
			m_epat_no = temp-100;
		}
	}
	if (p_INP != NULL || p_vstup != NULL)
		PrintInput();
	UpdateData(FALSE);
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

//********************************************************
//zistenie vitaza (najmensia aktivacia pri RBF neuronoch
int CLenetWDlg::GetOutput(void)
{
	
	int temp = 0;
		
		//_CLASSIC-1 je index predposlednej vrstvy 
		for (int i = 0 ; i < p_OUT->this_planes ; i++) {
			float value = 0.0;
			for (int j = 0 ; j < p_F[_CLASSICS-1]->this_planes ; j++) {
				value += (p_F[_CLASSICS-1]->plane[j].x-p_OUT->plane[i].w[j])*(p_F[_CLASSICS-1]->plane[j].x-p_OUT->plane[i].w[j]);
				//value += (p_OUT->plane[i].w[j]-p_F[_CLASSICS-1]->plane[j].x)*(p_OUT->plane[i].w[j]-p_F[_CLASSICS-1]->plane[j].x);
			}
			p_OUT->plane[i].x = value;
		}

		float min = p_OUT->plane[0].x;
	
		for (int i = 1 ; i < p_OUT->this_planes ; i++) {
			if ( min > p_OUT->plane[i].x) {
				temp = i;
				min = p_OUT->plane[i].x;
			}
		}
		return p_OUT->plane[temp].membership;
		
}

//********************************************************
// zistenie aktivacie na vystupe
float CLenetWDlg::get_output_activation(void)
{
	int temp = 0;
	float min = p_OUT->plane[0].x;
	
	for (int i = 1 ; i < p_OUT->this_planes ; i++) {
		if ( min > p_OUT->plane[i].x) {
			temp = i;
			min = p_OUT->plane[i].x;
		}
	}
	return min;
}

//********************************************************
// vykreslenie konvolucnych filtrov
void CLenetWDlg::PrintFields(void)
{
	CClientDC dc(this);
	int h, i, j, k, m, n, p_x, p_y;
	int temp;

	if ( info != NULL) {
		//nastavenie suradnicoveho systemu
		dc.SetViewportOrg(299,265);
		dc.Rectangle(-5,-5,590,145);
		//vykreslenie planov pre cely Lenet
		p_x = 0;
		p_y = 0;
		int convol = 0;
		int sampl = 0;
		int classic = 0;
		float value;
		for (int a = 0 ; a < _CONVOLUTIONS+_SAMPLINGS+_CLASSICS ; a++) {
					
			switch (info[a]) {
				case 'c':				
				
				for (i = 0 ; i < p_C[convol]->this_planes ; i++) {
					p_y = 0;
					for (h = 0 ; h < p_C[convol]->prev_planes ; h++) {
						//dc.Rectangle(p_x, p_y, p_x + p_C[convol]->receptiveM + 2, p_y + p_C[convol]->receptiveN + 2);
						for (m = 1 ; m <= p_C[convol]->receptiveM ; m++) {
							for (n = 1 ; n <= p_C[convol]->receptiveN ; n++) {
							
								
								if (p_C[convol]->plane[i].w[h][m-1][n-1] > 1.7)
									temp = 0;
								else if (p_C[convol]->plane[i].w[h][m-1][n-1] < -1.7)
									temp = 255;
								else
									temp = 255 - (int)((p_C[convol]->plane[i].w[h][m-1][n-1] + 1.7)*75);
								dc.SetPixelV(p_x + m, p_y + n, RGB(temp,temp,temp));
								
								
							}
						}
						p_y = p_y + p_C[convol]->receptiveM + 5;
					}
					p_x = p_x + p_C[convol]->receptiveN + 5;
				}
				p_x += p_C[convol]->receptiveN + 3; 
				convol++;

				break;
				
			}
			
		}
	}	
}
