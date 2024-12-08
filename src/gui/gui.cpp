///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "../StdInclude.h"

#include "gui.h"

#include "../icons/FootMeasurements_small.xpm"
#include "../icons/LegMeasurements.xpm"

///////////////////////////////////////////////////////////////////////////

GUIFrameMain::GUIFrameMain(wxDocument* doc, wxView* view, wxDocParentFrame* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDocChildFrame( doc, view, parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	m_menubar = new wxMenuBar( 0 );
	m_menuFile = new wxMenu();
	m_menubar->Append( m_menuFile, _("&File") );

	m_menuEdit = new wxMenu();
	m_menubar->Append( m_menuEdit, _("&Edit") );

	m_menuFoot = new wxMenu();
	wxMenuItem* m_menuItemQuickSetup;
	m_menuItemQuickSetup = new wxMenuItem( m_menuFoot, ID_QUICKSETUPMEASUREMENTS, wxString( _("Quick &Setup") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuFoot->Append( m_menuItemQuickSetup );

	m_menuFoot->AppendSeparator();

	m_menuFootParameters = new wxMenu();
	wxMenuItem* m_menuFootParametersItem = new wxMenuItem( m_menuFoot, wxID_ANY, _("Foot &parameters"), wxEmptyString, wxITEM_NORMAL, m_menuFootParameters );
	wxMenuItem* m_menuItemFootModelBone;
	m_menuItemFootModelBone = new wxMenuItem( m_menuFootParameters, ID_USEFOOTMEASUREMENTS, wxString( _("from &measurements") ) , wxEmptyString, wxITEM_RADIO );
	m_menuFootParameters->Append( m_menuItemFootModelBone );
	m_menuItemFootModelBone->Check( true );

	wxMenuItem* m_menuItemFootModelScan;
	m_menuItemFootModelScan = new wxMenuItem( m_menuFootParameters, ID_USEFOOTSCAN, wxString( _("from &scan") ) , wxEmptyString, wxITEM_RADIO );
	m_menuFootParameters->Append( m_menuItemFootModelScan );

	m_menuFoot->Append( m_menuFootParametersItem );

	wxMenuItem* m_menuItemSymmetry;
	m_menuItemSymmetry = new wxMenuItem( m_menuFoot, ID_MEASUREMENTSYMMETRY, wxString( _("Symmetric foot measurements") ) , wxEmptyString, wxITEM_CHECK );
	m_menuFoot->Append( m_menuItemSymmetry );
	m_menuItemSymmetry->Check( true );

	m_menuMeasurementsCopy = new wxMenu();
	wxMenuItem* m_menuMeasurementsCopyItem = new wxMenuItem( m_menuFoot, wxID_ANY, _("&Copy measurements"), wxEmptyString, wxITEM_NORMAL, m_menuMeasurementsCopy );
	wxMenuItem* m_menuItemMeasurementsCopyActiveToInactive;
	m_menuItemMeasurementsCopyActiveToInactive = new wxMenuItem( m_menuMeasurementsCopy, ID_COPYACTIVETOINACTIVE, wxString( _("from active side to inactive side") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuMeasurementsCopy->Append( m_menuItemMeasurementsCopyActiveToInactive );

	wxMenuItem* m_menuItemMeasurementsCopyRightToLeft;
	m_menuItemMeasurementsCopyRightToLeft = new wxMenuItem( m_menuMeasurementsCopy, ID_COPYRIGHTTOLEFT, wxString( _("from right to left") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuMeasurementsCopy->Append( m_menuItemMeasurementsCopyRightToLeft );

	wxMenuItem* m_menuItemMeasurementsCopyLeftToRight;
	m_menuItemMeasurementsCopyLeftToRight = new wxMenuItem( m_menuMeasurementsCopy, ID_COPYLEFTTORIGHT, wxString( _("from left to right") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuMeasurementsCopy->Append( m_menuItemMeasurementsCopyLeftToRight );

	m_menuFoot->Append( m_menuMeasurementsCopyItem );

	wxMenuItem* m_menuItemFootModelLoadSTL;
	m_menuItemFootModelLoadSTL = new wxMenuItem( m_menuFoot, wxID_ANY, wxString( _("Load foot scan") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuFoot->Append( m_menuItemFootModelLoadSTL );
	m_menuItemFootModelLoadSTL->Enable( false );

	m_menuFoot->AppendSeparator();

	m_menuFootModel = new wxMenu();
	wxMenuItem* m_menuFootModelItem = new wxMenuItem( m_menuFoot, wxID_ANY, _("Foot &model"), wxEmptyString, wxITEM_NORMAL, m_menuFootModel );
	wxMenuItem* m_menuItemBoneBasedModel;
	m_menuItemBoneBasedModel = new wxMenuItem( m_menuFootModel, ID_USEBONEBASEDMODEL, wxString( _("Bone-based model") ) , wxEmptyString, wxITEM_RADIO );
	m_menuFootModel->Append( m_menuItemBoneBasedModel );
	m_menuItemBoneBasedModel->Check( true );

	wxMenuItem* m_menuItemUseLastBasedModel;
	m_menuItemUseLastBasedModel = new wxMenuItem( m_menuFootModel, ID_USELASTBASEDMODEL, wxString( _("Last-based model") ) , wxEmptyString, wxITEM_RADIO );
	m_menuFootModel->Append( m_menuItemUseLastBasedModel );

	m_menuFoot->Append( m_menuFootModelItem );

	m_menuBoneModel = new wxMenu();
	wxMenuItem* m_menuBoneModelItem = new wxMenuItem( m_menuFoot, wxID_ANY, _("&Foot bone model"), wxEmptyString, wxITEM_NORMAL, m_menuBoneModel );
	wxMenuItem* m_menuItemEditBoneModel;
	m_menuItemEditBoneModel = new wxMenuItem( m_menuBoneModel, ID_EDITBONEMODEL, wxString( _("&Edit bone model") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuBoneModel->Append( m_menuItemEditBoneModel );

	wxMenuItem* m_menuItemLoadBoneModel;
	m_menuItemLoadBoneModel = new wxMenuItem( m_menuBoneModel, ID_LOADFOOTMODEL, wxString( _("&Load bone model") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuBoneModel->Append( m_menuItemLoadBoneModel );

	wxMenuItem* m_menuItemSaveBoneModel;
	m_menuItemSaveBoneModel = new wxMenuItem( m_menuBoneModel, ID_SAVEFOOTMODEL, wxString( _("&Save bone model") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuBoneModel->Append( m_menuItemSaveBoneModel );

	m_menuFoot->Append( m_menuBoneModelItem );

	wxMenuItem* m_menuItemLastModelLoad;
	m_menuItemLastModelLoad = new wxMenuItem( m_menuFoot, wxID_ANY, wxString( _("&Load Last model") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuFoot->Append( m_menuItemLastModelLoad );

	m_menubar->Append( m_menuFoot, _("F&oot") );

	m_menuLast = new wxMenu();
	wxMenuItem* m_menuItemLastLoadSTL;
	m_menuItemLastLoadSTL = new wxMenuItem( m_menuLast, wxID_ANY, wxString( _("Load Last from STL") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuLast->Append( m_menuItemLastLoadSTL );
	m_menuItemLastLoadSTL->Enable( false );

	m_menubar->Append( m_menuLast, _("&Last") );

	m_menuShoe = new wxMenu();
	m_menuConstruction = new wxMenu();
	wxMenuItem* m_menuConstructionItem = new wxMenuItem( m_menuShoe, wxID_ANY, _("&Construction method"), wxEmptyString, wxITEM_NORMAL, m_menuConstruction );
	wxMenuItem* m_menuItemTestGenerator;
	m_menuItemTestGenerator = new wxMenuItem( m_menuConstruction, ID_CONSTRUCTIONEXPERIMENTAL, wxString( _("&Experimental generator") ) , _("Generator for alogithm development"), wxITEM_RADIO );
	m_menuConstruction->Append( m_menuItemTestGenerator );

	wxMenuItem* m_menuItemConstructionClassic;
	m_menuItemConstructionClassic = new wxMenuItem( m_menuConstruction, ID_CONSTRUCTIONWELDED, wxString( _("Classic, &welded construction") ) , wxEmptyString, wxITEM_RADIO );
	m_menuConstruction->Append( m_menuItemConstructionClassic );

	wxMenuItem* m_menuItemConstructionCemented;
	m_menuItemConstructionCemented = new wxMenuItem( m_menuConstruction, ID_CONSTRUCTIONCEMENTED, wxString( _("&Cemented construction") ) , wxEmptyString, wxITEM_RADIO );
	m_menuConstruction->Append( m_menuItemConstructionCemented );

	wxMenuItem* m_menuItemConstructionMolded;
	m_menuItemConstructionMolded = new wxMenuItem( m_menuConstruction, ID_CONSTRUCTIONMOLDED, wxString( _("Injection-&molded sole") ) , wxEmptyString, wxITEM_RADIO );
	m_menuConstruction->Append( m_menuItemConstructionMolded );

	wxMenuItem* m_menuItemConstructionDutch;
	m_menuItemConstructionDutch = new wxMenuItem( m_menuConstruction, ID_CONSTRUCTIONDUTCH, wxString( _("&Dutch clog") ) , wxEmptyString, wxITEM_RADIO );
	m_menuConstruction->Append( m_menuItemConstructionDutch );

	wxMenuItem* m_menuItemConstructionGeta;
	m_menuItemConstructionGeta = new wxMenuItem( m_menuConstruction, ID_CONSTRUCTIONGETA, wxString( _("&Japanese Geta") ) , wxEmptyString, wxITEM_RADIO );
	m_menuConstruction->Append( m_menuItemConstructionGeta );

	m_menuShoe->Append( m_menuConstructionItem );

	m_menubar->Append( m_menuShoe, _("&Shoe") );

	m_menuPattern = new wxMenu();
	wxMenuItem* m_menuItemLoadPattern;
	m_menuItemLoadPattern = new wxMenuItem( m_menuPattern, ID_LOADPATTERN, wxString( _("&Load Pattern") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuPattern->Append( m_menuItemLoadPattern );
	m_menuItemLoadPattern->Enable( false );

	wxMenuItem* m_menuItemSavePattern;
	m_menuItemSavePattern = new wxMenuItem( m_menuPattern, ID_SAVEPATTERN, wxString( _("&Save Pattern") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuPattern->Append( m_menuItemSavePattern );
	m_menuItemSavePattern->Enable( false );

	wxMenuItem* m_menuItemShowFlattening;
	m_menuItemShowFlattening = new wxMenuItem( m_menuPattern, wxID_ANY, wxString( _("Show Flattening") ) , wxEmptyString, wxITEM_CHECK );
	m_menuPattern->Append( m_menuItemShowFlattening );
	m_menuItemShowFlattening->Enable( false );

	m_menubar->Append( m_menuPattern, _("&Pattern") );

	m_menuFabric = new wxMenu();
	wxMenuItem* m_menuItemEditFabricsLibrary;
	m_menuItemEditFabricsLibrary = new wxMenuItem( m_menuFabric, wxID_ANY, wxString( _("&Edit Fabrics Library") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuFabric->Append( m_menuItemEditFabricsLibrary );
	m_menuItemEditFabricsLibrary->Enable( false );

	m_menuFabric->AppendSeparator();

	wxMenuItem* m_menuItemLoadFabricsLibrary;
	m_menuItemLoadFabricsLibrary = new wxMenuItem( m_menuFabric, wxID_ANY, wxString( _("&Load Fabrics Library") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuFabric->Append( m_menuItemLoadFabricsLibrary );
	m_menuItemLoadFabricsLibrary->Enable( false );

	m_menubar->Append( m_menuFabric, _("F&abric") );

	m_menuGeometry = new wxMenu();
	wxMenuItem* m_menuItemSaveLast;
	m_menuItemSaveLast = new wxMenuItem( m_menuGeometry, wxID_ANY, wxString( _("Save &Last") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuGeometry->Append( m_menuItemSaveLast );

	wxMenuItem* m_menuItemSaveInsole;
	m_menuItemSaveInsole = new wxMenuItem( m_menuGeometry, wxID_ANY, wxString( _("Save &Insole") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuGeometry->Append( m_menuItemSaveInsole );
	m_menuItemSaveInsole->Enable( false );

	wxMenuItem* m_menuItemSaveSole;
	m_menuItemSaveSole = new wxMenuItem( m_menuGeometry, wxID_ANY, wxString( _("Save &Sole Prototype") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuGeometry->Append( m_menuItemSaveSole );
	m_menuItemSaveSole->Enable( false );

	wxMenuItem* m_menuItemCutaway;
	m_menuItemCutaway = new wxMenuItem( m_menuGeometry, wxID_ANY, wxString( _("Save &Cutaway Object") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuGeometry->Append( m_menuItemCutaway );
	m_menuItemCutaway->Enable( false );

	m_menuGeometry->AppendSeparator();

	wxMenuItem* m_menuItemPackZip;
	m_menuItemPackZip = new wxMenuItem( m_menuGeometry, wxID_ANY, wxString( _("Pack a Zip with &Everything") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuGeometry->Append( m_menuItemPackZip );
	m_menuItemPackZip->Enable( false );

	m_menubar->Append( m_menuGeometry, _("&Geometry") );

	m_menuView = new wxMenu();
	wxMenuItem* m_menuItemStereo3D;
	m_menuItemStereo3D = new wxMenuItem( m_menuView, ID_STEREO3D, wxString( _("Toggle Stereo &3D") ) + wxT('\t') + wxT("CTRL+3"), _("Toggle 3D view on/off."), wxITEM_CHECK );
	m_menuView->Append( m_menuItemStereo3D );

	m_menuView->AppendSeparator();

	wxMenuItem* m_menuItemShowLeft;
	m_menuItemShowLeft = new wxMenuItem( m_menuView, ID_SHOWLEFT, wxString( _("Show &Left") ) + wxT('\t') + wxT("Ctrl+E"), wxEmptyString, wxITEM_CHECK );
	m_menuView->Append( m_menuItemShowLeft );

	wxMenuItem* m_menuItemShowRight;
	m_menuItemShowRight = new wxMenuItem( m_menuView, ID_SHOWRIGHT, wxString( _("Show &Right") ) + wxT('\t') + wxT("Ctrl+R"), wxEmptyString, wxITEM_CHECK );
	m_menuView->Append( m_menuItemShowRight );

	m_menuView->AppendSeparator();

	wxMenuItem* m_menuItemShowBones;
	m_menuItemShowBones = new wxMenuItem( m_menuView, ID_SHOWBONES, wxString( _("Show &Bones") ) , wxEmptyString, wxITEM_CHECK );
	m_menuView->Append( m_menuItemShowBones );

	wxMenuItem* m_menuItemShowSkin;
	m_menuItemShowSkin = new wxMenuItem( m_menuView, ID_SHOWSKIN, wxString( _("Show &Skin") ) , wxEmptyString, wxITEM_CHECK );
	m_menuView->Append( m_menuItemShowSkin );

	wxMenuItem* m_menuItemShowLeg;
	m_menuItemShowLeg = new wxMenuItem( m_menuView, ID_SHOWLEG, wxString( _("Show L&eg") ) , wxEmptyString, wxITEM_CHECK );
	m_menuView->Append( m_menuItemShowLeg );
	m_menuItemShowLeg->Enable( false );

	wxMenuItem* m_menuItemShowLast;
	m_menuItemShowLast = new wxMenuItem( m_menuView, ID_SHOWLAST, wxString( _("Show L&ast") ) , wxEmptyString, wxITEM_CHECK );
	m_menuView->Append( m_menuItemShowLast );

	wxMenuItem* m_menuItemShowInsole;
	m_menuItemShowInsole = new wxMenuItem( m_menuView, ID_SHOWINSOLE, wxString( _("Show &Insole") ) , wxEmptyString, wxITEM_CHECK );
	m_menuView->Append( m_menuItemShowInsole );

	wxMenuItem* m_menuIItemShowSole;
	m_menuIItemShowSole = new wxMenuItem( m_menuView, ID_SHOWSOLE, wxString( _("Show S&ole") ) , wxEmptyString, wxITEM_CHECK );
	m_menuView->Append( m_menuIItemShowSole );

	wxMenuItem* m_menuItemShowUpper;
	m_menuItemShowUpper = new wxMenuItem( m_menuView, ID_SHOWUPPER, wxString( _("Show &Upper") ) , wxEmptyString, wxITEM_CHECK );
	m_menuView->Append( m_menuItemShowUpper );

	wxMenuItem* m_menuItemShowCutaway;
	m_menuItemShowCutaway = new wxMenuItem( m_menuView, ID_SHOWCUTAWAY, wxString( _("Show &Cutaway Block") ) , wxEmptyString, wxITEM_CHECK );
	m_menuView->Append( m_menuItemShowCutaway );

	wxMenuItem* m_menuItemShowFloor;
	m_menuItemShowFloor = new wxMenuItem( m_menuView, ID_SHOWFLOOR, wxString( _("Show &Floor") ) , wxEmptyString, wxITEM_CHECK );
	m_menuView->Append( m_menuItemShowFloor );

	m_menuView->AppendSeparator();

	wxMenuItem* m_menuItemShowCoordinateSystem;
	m_menuItemShowCoordinateSystem = new wxMenuItem( m_menuView, ID_SHOWCOORDINATESYSTEM, wxString( _("Show Coor&dinate System") ) , wxEmptyString, wxITEM_CHECK );
	m_menuView->Append( m_menuItemShowCoordinateSystem );

	wxMenuItem* m_menuItemShowBackground;
	m_menuItemShowBackground = new wxMenuItem( m_menuView, ID_SHOWBACKGROUND, wxString( _("Show Back&ground") ) + wxT('\t') + wxT("CTRL+B"), wxEmptyString, wxITEM_CHECK );
	m_menuView->Append( m_menuItemShowBackground );

	m_menuView->AppendSeparator();

	wxMenuItem* m_menuItemSetupBackground;
	m_menuItemSetupBackground = new wxMenuItem( m_menuView, wxID_ANY, wxString( _("Setup Background Images") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuView->Append( m_menuItemSetupBackground );

	m_menubar->Append( m_menuView, _("&View") );

	m_menuPreferences = new wxMenu();
	m_menubar->Append( m_menuPreferences, _("P&references") );

	m_menuHelp = new wxMenu();
	wxMenuItem* m_menuItemdebugParser;
	m_menuItemdebugParser = new wxMenuItem( m_menuHelp, wxID_ANY, wxString( _("Debug Mathparser") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuHelp->Append( m_menuItemdebugParser );

	m_menubar->Append( m_menuHelp, _("&Help") );

	this->SetMenuBar( m_menubar );

	wxBoxSizer* bSizer;
	bSizer = new wxBoxSizer( wxVERTICAL );

	m_splitter = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter->Connect( wxEVT_IDLE, wxIdleEventHandler( GUIFrameMain::m_splitterOnIdle ), NULL, this );
	m_splitter->SetMinimumPaneSize( 300 );

	m_panelConfig = new wxPanel( m_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerConfig;
	bSizerConfig = new wxBoxSizer( wxVERTICAL );

	m_notebook = new wxNotebook( m_panelConfig, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panelPageFoot = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerFoot;
	bSizerFoot = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizerEditLeftRight;
	bSizerEditLeftRight = new wxBoxSizer( wxHORIZONTAL );

	m_toggleBtnEditLeft = new wxToggleButton( m_panelPageFoot, ID_EDITLEFT, _("Left"), wxDefaultPosition, wxDefaultSize, 0 );
	m_toggleBtnEditLeft->SetValue( true );
	m_toggleBtnEditLeft->SetToolTip( _("Edit left foots measurements.") );

	bSizerEditLeftRight->Add( m_toggleBtnEditLeft, 1, wxALL, 5 );

	m_toggleBtnEditRight = new wxToggleButton( m_panelPageFoot, ID_EDITRIGHT, _("Right"), wxDefaultPosition, wxDefaultSize, 0 );
	m_toggleBtnEditRight->SetToolTip( _("Edit right foots measurements.") );

	bSizerEditLeftRight->Add( m_toggleBtnEditRight, 1, wxALL, 5 );


	bSizerFoot->Add( bSizerEditLeftRight, 0, wxEXPAND, 5 );

	m_choicebookMeasurement = new wxChoicebook( m_panelPageFoot, ID_MEASUREMENTSOURCE, wxDefaultPosition, wxDefaultSize, wxCHB_DEFAULT );
	m_panelMeasurementBased = new wxPanel( m_choicebookMeasurement, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerFootMeasurements;
	bSizerFootMeasurements = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizerFootMeasurementsHorizontal;
	bSizerFootMeasurementsHorizontal = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizerFootMeasurementsVertical;
	bSizerFootMeasurementsVertical = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizerFootMeasurements;
	fgSizerFootMeasurements = new wxFlexGridSizer( 9, 2, 0, 0 );
	fgSizerFootMeasurements->AddGrowableCol( 1 );
	fgSizerFootMeasurements->SetFlexibleDirection( wxHORIZONTAL );
	fgSizerFootMeasurements->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextFootLength = new wxStaticText( m_panelMeasurementBased, wxID_ANY, _("Foot length:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextFootLength->Wrap( -1 );
	fgSizerFootMeasurements->Add( m_staticTextFootLength, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_textCtrlFootLength = new ExtendedTextCtrl( m_panelMeasurementBased, ID_MEASUREMENT_FOOTLENGTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerFootMeasurements->Add( m_textCtrlFootLength, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );

	m_staticTextBallWidth = new wxStaticText( m_panelMeasurementBased, wxID_ANY, _("Ball width:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextBallWidth->Wrap( -1 );
	fgSizerFootMeasurements->Add( m_staticTextBallWidth, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrlBallWidth = new ExtendedTextCtrl( m_panelMeasurementBased, ID_MEASUREMENT_BALLWIDTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerFootMeasurements->Add( m_textCtrlBallWidth, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );

	m_staticTextBallGirth = new wxStaticText( m_panelMeasurementBased, wxID_ANY, _("Big toe girth:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextBallGirth->Wrap( -1 );
	fgSizerFootMeasurements->Add( m_staticTextBallGirth, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrlBigToeGirth = new ExtendedTextCtrl( m_panelMeasurementBased, ID_MEASUREMENT_BIGTOEGIRTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerFootMeasurements->Add( m_textCtrlBigToeGirth, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticTextWaistGirth = new wxStaticText( m_panelMeasurementBased, wxID_ANY, _("Little toe girth:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextWaistGirth->Wrap( -1 );
	fgSizerFootMeasurements->Add( m_staticTextWaistGirth, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_textCtrlLittleToeGirth = new ExtendedTextCtrl( m_panelMeasurementBased, ID_MEASUREMENT_LITTLETOEGIRTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerFootMeasurements->Add( m_textCtrlLittleToeGirth, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticTextInsteGirth = new wxStaticText( m_panelMeasurementBased, wxID_ANY, _("Waist girth:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextInsteGirth->Wrap( -1 );
	fgSizerFootMeasurements->Add( m_staticTextInsteGirth, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_textCtrlWaistGirth = new ExtendedTextCtrl( m_panelMeasurementBased, ID_MEASUREMENT_WAISTGIRTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerFootMeasurements->Add( m_textCtrlWaistGirth, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );

	m_staticTextLongHeelGirth = new wxStaticText( m_panelMeasurementBased, wxID_ANY, _("Heel girth:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextLongHeelGirth->Wrap( -1 );
	fgSizerFootMeasurements->Add( m_staticTextLongHeelGirth, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrlHeelGirth = new ExtendedTextCtrl( m_panelMeasurementBased, ID_MEASUREMENT_HEELGIRTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerFootMeasurements->Add( m_textCtrlHeelGirth, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );

	m_staticTextShortHeelGirth = new wxStaticText( m_panelMeasurementBased, wxID_ANY, _("Heel width:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextShortHeelGirth->Wrap( -1 );
	fgSizerFootMeasurements->Add( m_staticTextShortHeelGirth, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrlHeelWidth = new ExtendedTextCtrl( m_panelMeasurementBased, ID_MEASUREMENT_HEELWIDTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerFootMeasurements->Add( m_textCtrlHeelWidth, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );

	m_staticTextAngleMixing = new wxStaticText( m_panelMeasurementBased, wxID_ANY, _("Angle Mixing:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextAngleMixing->Wrap( -1 );
	fgSizerFootMeasurements->Add( m_staticTextAngleMixing, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_textCtrlAngleMixing = new ExtendedTextCtrl( m_panelMeasurementBased, ID_MEASUREMENT_ANGLEMIXING, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerFootMeasurements->Add( m_textCtrlAngleMixing, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );

	m_staticTextLegLengthDifference = new wxStaticText( m_panelMeasurementBased, wxID_ANY, _("Leg length difference:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextLegLengthDifference->Wrap( -1 );
	fgSizerFootMeasurements->Add( m_staticTextLegLengthDifference, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_textCtrlLegLengthDifference = new ExtendedTextCtrl( m_panelMeasurementBased, ID_MEASUREMENT_LEGLENGTHDIFFERENCE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerFootMeasurements->Add( m_textCtrlLegLengthDifference, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );


	bSizerFootMeasurementsVertical->Add( fgSizerFootMeasurements, 0, wxEXPAND, 5 );


	bSizerFootMeasurementsHorizontal->Add( bSizerFootMeasurementsVertical, 1, 0, 5 );

	wxBoxSizer* bSizerShoeSizes;
	bSizerShoeSizes = new wxBoxSizer( wxVERTICAL );

	m_buttonQuickSetup = new wxButton( m_panelMeasurementBased, ID_QUICKSETUPMEASUREMENTS, _("Quick Setup"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerShoeSizes->Add( m_buttonQuickSetup, 0, wxALL|wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizerShoeSizes;
	sbSizerShoeSizes = new wxStaticBoxSizer( new wxStaticBox( m_panelMeasurementBased, wxID_ANY, _("Shoesizes") ), wxVERTICAL );

	wxFlexGridSizer* fgSizerShoeSizes;
	fgSizerShoeSizes = new wxFlexGridSizer( 6, 2, 0, 0 );
	fgSizerShoeSizes->AddGrowableCol( 1 );
	fgSizerShoeSizes->SetFlexibleDirection( wxHORIZONTAL );
	fgSizerShoeSizes->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextShoeSizeEU = new wxStaticText( sbSizerShoeSizes->GetStaticBox(), wxID_ANY, _("EU:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextShoeSizeEU->Wrap( -1 );
	fgSizerShoeSizes->Add( m_staticTextShoeSizeEU, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_textCtrlShoeSizeEU = new ExtendedTextCtrl( sbSizerShoeSizes->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTER );
	m_textCtrlShoeSizeEU->SetToolTip( _("Last length in Paris point") );

	fgSizerShoeSizes->Add( m_textCtrlShoeSizeEU, 0, wxALL|wxEXPAND, 5 );

	m_staticTextShoeSizeUS = new wxStaticText( sbSizerShoeSizes->GetStaticBox(), wxID_ANY, _("US:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextShoeSizeUS->Wrap( -1 );
	fgSizerShoeSizes->Add( m_staticTextShoeSizeUS, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrlShoeSizeUS = new ExtendedTextCtrl( sbSizerShoeSizes->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTER );
	m_textCtrlShoeSizeUS->SetToolTip( _("Brannock-Scale") );

	fgSizerShoeSizes->Add( m_textCtrlShoeSizeUS, 0, wxALL|wxEXPAND, 5 );

	m_staticTextShoeSizeUK = new wxStaticText( sbSizerShoeSizes->GetStaticBox(), wxID_ANY, _("UK:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextShoeSizeUK->Wrap( -1 );
	fgSizerShoeSizes->Add( m_staticTextShoeSizeUK, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrlShoeSizeUK = new ExtendedTextCtrl( sbSizerShoeSizes->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTER );
	m_textCtrlShoeSizeUK->SetToolTip( _("Last length in Barleycorns") );

	fgSizerShoeSizes->Add( m_textCtrlShoeSizeUK, 0, wxALL|wxEXPAND, 5 );

	m_staticTextShoeSizeCN = new wxStaticText( sbSizerShoeSizes->GetStaticBox(), wxID_ANY, _("CN:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextShoeSizeCN->Wrap( -1 );
	fgSizerShoeSizes->Add( m_staticTextShoeSizeCN, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_textCtrlShoeSizeCN = new ExtendedTextCtrl( sbSizerShoeSizes->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTER );
	m_textCtrlShoeSizeCN->SetToolTip( _("Foot length in cm") );

	fgSizerShoeSizes->Add( m_textCtrlShoeSizeCN, 0, wxALL|wxEXPAND, 5 );

	m_staticTextShoeSizeJP = new wxStaticText( sbSizerShoeSizes->GetStaticBox(), wxID_ANY, _("JP:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextShoeSizeJP->Wrap( -1 );
	fgSizerShoeSizes->Add( m_staticTextShoeSizeJP, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrlShoeSizeJP = new ExtendedTextCtrl( sbSizerShoeSizes->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTER );
	m_textCtrlShoeSizeJP->SetToolTip( _("Foot length in mm") );

	fgSizerShoeSizes->Add( m_textCtrlShoeSizeJP, 0, wxALL|wxEXPAND, 5 );

	m_staticTextShoeSizeAU = new wxStaticText( sbSizerShoeSizes->GetStaticBox(), wxID_ANY, _("AU:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextShoeSizeAU->Wrap( -1 );
	fgSizerShoeSizes->Add( m_staticTextShoeSizeAU, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrlShoeSizeAU = new ExtendedTextCtrl( sbSizerShoeSizes->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTER );
	m_textCtrlShoeSizeAU->SetToolTip( _("Last length in Barleycorns") );

	fgSizerShoeSizes->Add( m_textCtrlShoeSizeAU, 0, wxALL|wxEXPAND, 5 );


	sbSizerShoeSizes->Add( fgSizerShoeSizes, 0, wxALIGN_CENTER_HORIZONTAL, 5 );


	bSizerShoeSizes->Add( sbSizerShoeSizes, 0, 0, 5 );


	bSizerFootMeasurementsHorizontal->Add( bSizerShoeSizes, 0, 0, 5 );


	bSizerFootMeasurements->Add( bSizerFootMeasurementsHorizontal, 0, wxEXPAND, 5 );

	m_bitmapFoot = new wxStaticBitmap( m_panelMeasurementBased, ID_IMAGEFOOT, wxBitmap( FootMeasurements_small_xpm ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerFootMeasurements->Add( m_bitmapFoot, 0, wxEXPAND, 5 );


	m_panelMeasurementBased->SetSizer( bSizerFootMeasurements );
	m_panelMeasurementBased->Layout();
	bSizerFootMeasurements->Fit( m_panelMeasurementBased );
	m_choicebookMeasurement->AddPage( m_panelMeasurementBased, _("Measurement based"), true );
	m_panelScanBased = new wxPanel( m_choicebookMeasurement, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerScanBasedModel;
	bSizerScanBasedModel = new wxBoxSizer( wxVERTICAL );

	m_staticTextScanFile = new wxStaticText( m_panelScanBased, wxID_ANY, _("Scan file:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextScanFile->Wrap( -1 );
	bSizerScanBasedModel->Add( m_staticTextScanFile, 0, wxALL, 5 );

	m_filePickerScanFile = new wxFilePickerCtrl( m_panelScanBased, wxID_ANY, wxEmptyString, _("Select a file"), _("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	bSizerScanBasedModel->Add( m_filePickerScanFile, 0, wxALL|wxEXPAND, 5 );


	m_panelScanBased->SetSizer( bSizerScanBasedModel );
	m_panelScanBased->Layout();
	bSizerScanBasedModel->Fit( m_panelScanBased );
	m_choicebookMeasurement->AddPage( m_panelScanBased, _("Scan based"), false );
	bSizerFoot->Add( m_choicebookMeasurement, 1, wxEXPAND | wxALL, 5 );

	m_choicebookFootModel = new wxChoicebook( m_panelPageFoot, ID_MODELTYPE, wxDefaultPosition, wxDefaultSize, wxCHB_DEFAULT );
	m_panelBoneBasedModel = new wxPanel( m_choicebookFootModel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerBoneBasedModel;
	bSizerBoneBasedModel = new wxBoxSizer( wxVERTICAL );

	m_buttonEditBoneModel = new wxButton( m_panelBoneBasedModel, ID_EDITBONEMODEL, _("Edit model"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerBoneBasedModel->Add( m_buttonEditBoneModel, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );


	m_panelBoneBasedModel->SetSizer( bSizerBoneBasedModel );
	m_panelBoneBasedModel->Layout();
	bSizerBoneBasedModel->Fit( m_panelBoneBasedModel );
	m_choicebookFootModel->AddPage( m_panelBoneBasedModel, _("Bone based model"), false );
	m_panelLastBasedModel = new wxPanel( m_choicebookFootModel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerLastBasedModel;
	bSizerLastBasedModel = new wxBoxSizer( wxVERTICAL );

	m_staticTextLastModel = new wxStaticText( m_panelLastBasedModel, wxID_ANY, _("Last Model:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextLastModel->Wrap( -1 );
	bSizerLastBasedModel->Add( m_staticTextLastModel, 0, wxALL, 5 );

	m_filePickerLastModel = new wxFilePickerCtrl( m_panelLastBasedModel, ID_LASTFILENAME, wxEmptyString, _("Select last model"), _("3D Models (*.stl, *.obj)|*.stl; *.obj|Stereolitography models(*.stl)|*.stl|Wavefront objects (*.obj)|*.obj|All files|*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	bSizerLastBasedModel->Add( m_filePickerLastModel, 0, wxALL|wxEXPAND, 5 );


	m_panelLastBasedModel->SetSizer( bSizerLastBasedModel );
	m_panelLastBasedModel->Layout();
	bSizerLastBasedModel->Fit( m_panelLastBasedModel );
	m_choicebookFootModel->AddPage( m_panelLastBasedModel, _("Last based model"), true );
	bSizerFoot->Add( m_choicebookFootModel, 0, wxALL|wxEXPAND, 5 );


	m_panelPageFoot->SetSizer( bSizerFoot );
	m_panelPageFoot->Layout();
	bSizerFoot->Fit( m_panelPageFoot );
	m_notebook->AddPage( m_panelPageFoot, _("Foot"), true );
	m_panelPageLeg = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerLeg;
	bSizerLeg = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizerLeftRight1;
	bSizerLeftRight1 = new wxBoxSizer( wxHORIZONTAL );

	m_toggleBtnEditLeft1 = new wxToggleButton( m_panelPageLeg, ID_EDITLEFT, _("Left"), wxDefaultPosition, wxDefaultSize, 0 );
	m_toggleBtnEditLeft1->SetValue( true );
	m_toggleBtnEditLeft1->SetToolTip( _("Edit left legs measurements.") );

	bSizerLeftRight1->Add( m_toggleBtnEditLeft1, 1, wxALL, 5 );

	m_toggleBtnEditRight1 = new wxToggleButton( m_panelPageLeg, ID_EDITRIGHT, _("Right"), wxDefaultPosition, wxDefaultSize, 0 );
	m_toggleBtnEditRight1->SetToolTip( _("Edit right legs measurements.") );

	bSizerLeftRight1->Add( m_toggleBtnEditRight1, 1, wxALL, 5 );


	bSizerLeg->Add( bSizerLeftRight1, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizerLegMeasurements;
	bSizerLegMeasurements = new wxBoxSizer( wxHORIZONTAL );

	m_bitmapLeg = new wxStaticBitmap( m_panelPageLeg, wxID_ANY, wxBitmap( LegMeasurements_xpm ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerLegMeasurements->Add( m_bitmapLeg, 0, wxALL, 5 );

	wxBoxSizer* bSizerLegMeasurementsVertical;
	bSizerLegMeasurementsVertical = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizerLegMeasurements;
	fgSizerLegMeasurements = new wxFlexGridSizer( 9, 2, 0, 0 );
	fgSizerLegMeasurements->AddGrowableCol( 0 );
	fgSizerLegMeasurements->AddGrowableCol( 1 );
	fgSizerLegMeasurements->SetFlexibleDirection( wxHORIZONTAL );
	fgSizerLegMeasurements->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );

	m_staticTextGirth = new wxStaticText( m_panelPageLeg, wxID_ANY, _("Girth:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextGirth->Wrap( -1 );
	fgSizerLegMeasurements->Add( m_staticTextGirth, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticTextLevel = new wxStaticText( m_panelPageLeg, wxID_ANY, _("Level:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextLevel->Wrap( -1 );
	fgSizerLegMeasurements->Add( m_staticTextLevel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrlBelowCrutchGirth = new ExtendedTextCtrl( m_panelPageLeg, ID_MEASUREMENT_BELOWCRUTCHGIRTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerLegMeasurements->Add( m_textCtrlBelowCrutchGirth, 0, wxEXPAND|wxALL, 5 );

	m_textCtrlBelowCrutchLevel = new ExtendedTextCtrl( m_panelPageLeg, ID_MEASUREMENT_BELOWCRUTCHLEVEL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerLegMeasurements->Add( m_textCtrlBelowCrutchLevel, 0, wxALL|wxEXPAND, 5 );

	m_textCtrlMiddleOfCalfGirth = new ExtendedTextCtrl( m_panelPageLeg, ID_MEASUREMENT_MIDDLEOFCALFGIRTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerLegMeasurements->Add( m_textCtrlMiddleOfCalfGirth, 0, wxALL|wxEXPAND, 5 );

	m_textCtrlMiddleOfCalfLevel = new ExtendedTextCtrl( m_panelPageLeg, ID_MEASUREMENT_MIDDLEOFCALFLEVEL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerLegMeasurements->Add( m_textCtrlMiddleOfCalfLevel, 0, wxALL|wxEXPAND, 5 );

	m_textCtrlAboveKneeGirth = new ExtendedTextCtrl( m_panelPageLeg, ID_MEASUREMENT_ABOVEKNEEGIRTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerLegMeasurements->Add( m_textCtrlAboveKneeGirth, 0, wxALL|wxEXPAND, 5 );

	m_textCtrlAboveKneeLevel = new ExtendedTextCtrl( m_panelPageLeg, ID_MEASUREMENT_ABOVEKNEELEVEL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerLegMeasurements->Add( m_textCtrlAboveKneeLevel, 0, wxALL|wxEXPAND, 5 );

	m_textCtrlOverKneeCapGirth = new ExtendedTextCtrl( m_panelPageLeg, ID_MEASUREMENT_OVERKNEECAPGIRTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerLegMeasurements->Add( m_textCtrlOverKneeCapGirth, 0, wxALL|wxEXPAND, 5 );

	m_textCtrlOverKneeCapLevel = new ExtendedTextCtrl( m_panelPageLeg, ID_MEASUREMENT_OVERKNEECAPLEVEL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerLegMeasurements->Add( m_textCtrlOverKneeCapLevel, 0, wxALL|wxEXPAND, 5 );

	m_textCtrlBelowKneeGirth = new ExtendedTextCtrl( m_panelPageLeg, ID_MEASUREMENT_BELOWKNEEGIRTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerLegMeasurements->Add( m_textCtrlBelowKneeGirth, 0, wxALL|wxEXPAND, 5 );

	m_textCtrlBelowKneeLevel = new ExtendedTextCtrl( m_panelPageLeg, ID_MEASUREMENT_BELOWKNEELEVEL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerLegMeasurements->Add( m_textCtrlBelowKneeLevel, 0, wxALL|wxEXPAND, 5 );

	m_textCtrlMiddleOfShankGirth = new ExtendedTextCtrl( m_panelPageLeg, ID_MEASUREMENT_MIDDLEOFSHANKGIRTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerLegMeasurements->Add( m_textCtrlMiddleOfShankGirth, 0, wxALL|wxEXPAND, 5 );

	m_textCtrlMiddleOfShankLevel = new ExtendedTextCtrl( m_panelPageLeg, ID_MEASUREMENT_MIDDLEOFSHANKLEVEL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerLegMeasurements->Add( m_textCtrlMiddleOfShankLevel, 0, wxALL|wxEXPAND, 5 );

	m_textCtrlAboveAnkleGirth = new ExtendedTextCtrl( m_panelPageLeg, ID_MEASUREMENT_ABOVEANKLEGIRTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerLegMeasurements->Add( m_textCtrlAboveAnkleGirth, 0, wxALL|wxEXPAND, 5 );

	m_textCtrlAboveAnkleLevel = new ExtendedTextCtrl( m_panelPageLeg, ID_MEASUREMENT_ABOVEANKLELEVEL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerLegMeasurements->Add( m_textCtrlAboveAnkleLevel, 0, wxALL|wxEXPAND, 5 );

	m_textCtrlOverAnkleBoneGirth = new ExtendedTextCtrl( m_panelPageLeg, ID_MEASUREMENT_OVERANKLEBONEGIRTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerLegMeasurements->Add( m_textCtrlOverAnkleBoneGirth, 0, wxALL|wxEXPAND, 5 );

	m_textCtrlOverAnkleBoneLevel = new ExtendedTextCtrl( m_panelPageLeg, ID_MEASUREMENT_OVERANKLEBONELEVEL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerLegMeasurements->Add( m_textCtrlOverAnkleBoneLevel, 0, wxALL|wxEXPAND, 5 );


	bSizerLegMeasurementsVertical->Add( fgSizerLegMeasurements, 0, wxEXPAND, 5 );


	bSizerLegMeasurements->Add( bSizerLegMeasurementsVertical, 1, wxALIGN_CENTER_VERTICAL, 5 );


	bSizerLeg->Add( bSizerLegMeasurements, 0, wxEXPAND, 5 );


	m_panelPageLeg->SetSizer( bSizerLeg );
	m_panelPageLeg->Layout();
	bSizerLeg->Fit( m_panelPageLeg );
	m_notebook->AddPage( m_panelPageLeg, _("Leg"), false );
	m_panelPageInsole = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer53;
	bSizer53 = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* sbSizerInsoleShape;
	sbSizerInsoleShape = new wxStaticBoxSizer( new wxStaticBox( m_panelPageInsole, wxID_ANY, _("Insole shape") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer6;
	fgSizer6 = new wxFlexGridSizer( 7, 2, 0, 0 );
	fgSizer6->AddGrowableCol( 1 );
	fgSizer6->SetFlexibleDirection( wxBOTH );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextTipSymmetry = new wxStaticText( sbSizerInsoleShape->GetStaticBox(), wxID_ANY, _("Big toe angle:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextTipSymmetry->Wrap( -1 );
	fgSizer6->Add( m_staticTextTipSymmetry, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_textCtrlBigToeAngle = new ExtendedTextCtrl( sbSizerInsoleShape->GetStaticBox(), ID_BIGTOEANGLE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizer6->Add( m_textCtrlBigToeAngle, 0, wxALL|wxEXPAND, 5 );

	m_staticTextTipPointedness = new wxStaticText( sbSizerInsoleShape->GetStaticBox(), wxID_ANY, _("Little toe angle:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextTipPointedness->Wrap( -1 );
	fgSizer6->Add( m_staticTextTipPointedness, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_textCtrlLittleToeAngle = new ExtendedTextCtrl( sbSizerInsoleShape->GetStaticBox(), ID_LITTLETOEANGLE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizer6->Add( m_textCtrlLittleToeAngle, 0, wxALL|wxEXPAND, 5 );

	m_staticText57 = new wxStaticText( sbSizerInsoleShape->GetStaticBox(), wxID_ANY, _("Ball measurement angle:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText57->Wrap( -1 );
	fgSizer6->Add( m_staticText57, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_textCtrlBallMeasurementAngle = new ExtendedTextCtrl( sbSizerInsoleShape->GetStaticBox(), ID_BALLMEASUREMENTANGLE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizer6->Add( m_textCtrlBallMeasurementAngle, 0, wxALL|wxEXPAND, 5 );

	m_staticText58 = new wxStaticText( sbSizerInsoleShape->GetStaticBox(), wxID_ANY, _("Heel direction angle:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText58->Wrap( -1 );
	fgSizer6->Add( m_staticText58, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_textCtrlHeelDirectionAngle = new ExtendedTextCtrl( sbSizerInsoleShape->GetStaticBox(), ID_HEELDIRECTIONANGLE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizer6->Add( m_textCtrlHeelDirectionAngle, 0, wxALL|wxEXPAND, 5 );

	m_staticTextTipSharpness = new wxStaticText( sbSizerInsoleShape->GetStaticBox(), wxID_ANY, _("Tip sharpness:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextTipSharpness->Wrap( -1 );
	fgSizer6->Add( m_staticTextTipSharpness, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_sliderTipSharpness = new wxSlider( sbSizerInsoleShape->GetStaticBox(), ID_TIPSHARPNESS, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	fgSizer6->Add( m_sliderTipSharpness, 0, wxALL|wxEXPAND, 5 );

	m_staticTextExtraLength = new wxStaticText( sbSizerInsoleShape->GetStaticBox(), wxID_ANY, _("Extra length:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextExtraLength->Wrap( -1 );
	fgSizer6->Add( m_staticTextExtraLength, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_textCtrlExtraLength = new ExtendedTextCtrl( sbSizerInsoleShape->GetStaticBox(), ID_EXTRALENGTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizer6->Add( m_textCtrlExtraLength, 0, wxALL|wxEXPAND, 5 );

	m_staticTextFootCompression = new wxStaticText( sbSizerInsoleShape->GetStaticBox(), wxID_ANY, _("Foot compression:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextFootCompression->Wrap( -1 );
	fgSizer6->Add( m_staticTextFootCompression, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_textCtrlFootCompression = new ExtendedTextCtrl( sbSizerInsoleShape->GetStaticBox(), ID_FOOTCOMPRESSION, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizer6->Add( m_textCtrlFootCompression, 0, wxALL|wxEXPAND, 5 );


	sbSizerInsoleShape->Add( fgSizer6, 1, wxEXPAND, 5 );


	bSizer53->Add( sbSizerInsoleShape, 0, wxEXPAND, 5 );


	m_panelPageInsole->SetSizer( bSizer53 );
	m_panelPageInsole->Layout();
	bSizer53->Fit( m_panelPageInsole );
	m_notebook->AddPage( m_panelPageInsole, _("Insole"), false );
	m_panelPageShoe = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerShoe;
	bSizerShoe = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* sbSizerPresetType;
	sbSizerPresetType = new wxStaticBoxSizer( new wxStaticBox( m_panelPageShoe, wxID_ANY, _("Heel style") ), wxVERTICAL );

	wxArrayString m_choiceShoeTypeChoices;
	m_choiceShoeType = new wxChoice( sbSizerPresetType->GetStaticBox(), ID_PRESETSHOETYPE, wxDefaultPosition, wxDefaultSize, m_choiceShoeTypeChoices, 0 );
	m_choiceShoeType->SetSelection( 0 );
	sbSizerPresetType->Add( m_choiceShoeType, 0, wxALL|wxEXPAND, 5 );

	wxFlexGridSizer* fgSizerShoeBaseMeasurements;
	fgSizerShoeBaseMeasurements = new wxFlexGridSizer( 4, 2, 0, 0 );
	fgSizerShoeBaseMeasurements->AddGrowableCol( 1 );
	fgSizerShoeBaseMeasurements->SetFlexibleDirection( wxHORIZONTAL );
	fgSizerShoeBaseMeasurements->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextHeelHeight = new wxStaticText( sbSizerPresetType->GetStaticBox(), wxID_ANY, _("Heel Height:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextHeelHeight->Wrap( -1 );
	fgSizerShoeBaseMeasurements->Add( m_staticTextHeelHeight, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_textCtrlHeelHeight = new ExtendedTextCtrl( sbSizerPresetType->GetStaticBox(), ID_HEELHEIGHT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerShoeBaseMeasurements->Add( m_textCtrlHeelHeight, 0, wxALL|wxEXPAND, 5 );

	m_staticTextBallHeight = new wxStaticText( sbSizerPresetType->GetStaticBox(), wxID_ANY, _("Ball Height:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextBallHeight->Wrap( -1 );
	fgSizerShoeBaseMeasurements->Add( m_staticTextBallHeight, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_textCtrlBallHeight = new ExtendedTextCtrl( sbSizerPresetType->GetStaticBox(), ID_BALLHEIGHT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerShoeBaseMeasurements->Add( m_textCtrlBallHeight, 0, wxALL|wxEXPAND, 5 );

	m_staticTextHeelPitch = new wxStaticText( sbSizerPresetType->GetStaticBox(), wxID_ANY, _("Heel Pitch:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextHeelPitch->Wrap( -1 );
	fgSizerShoeBaseMeasurements->Add( m_staticTextHeelPitch, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrlHeelPitch = new ExtendedTextCtrl( sbSizerPresetType->GetStaticBox(), ID_HEELPITCH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerShoeBaseMeasurements->Add( m_textCtrlHeelPitch, 0, wxALL|wxEXPAND, 5 );

	m_staticTextToeSpring = new wxStaticText( sbSizerPresetType->GetStaticBox(), wxID_ANY, _("Toe Spring:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextToeSpring->Wrap( -1 );
	fgSizerShoeBaseMeasurements->Add( m_staticTextToeSpring, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_textCtrlToeSpring = new ExtendedTextCtrl( sbSizerPresetType->GetStaticBox(), ID_TOESPRING, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerShoeBaseMeasurements->Add( m_textCtrlToeSpring, 0, wxALL|wxEXPAND, 5 );


	sbSizerPresetType->Add( fgSizerShoeBaseMeasurements, 0, wxEXPAND, 5 );


	bSizerShoe->Add( sbSizerPresetType, 0, wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizerShoeHeight;
	sbSizerShoeHeight = new wxStaticBoxSizer( new wxStaticBox( m_panelPageShoe, wxID_ANY, _("Shoe style") ), wxVERTICAL );

	wxArrayString m_choiceShoeHeightChoices;
	m_choiceShoeHeight = new wxChoice( sbSizerShoeHeight->GetStaticBox(), ID_PRESETSHOEHEIGHT, wxDefaultPosition, wxDefaultSize, m_choiceShoeHeightChoices, 0 );
	m_choiceShoeHeight->SetSelection( 1 );
	sbSizerShoeHeight->Add( m_choiceShoeHeight, 0, wxALL|wxEXPAND, 5 );

	wxFlexGridSizer* fgSizerUpperLevel;
	fgSizerUpperLevel = new wxFlexGridSizer( 1, 2, 0, 0 );
	fgSizerUpperLevel->AddGrowableCol( 1 );
	fgSizerUpperLevel->SetFlexibleDirection( wxHORIZONTAL );
	fgSizerUpperLevel->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextUpperLevel = new wxStaticText( sbSizerShoeHeight->GetStaticBox(), wxID_ANY, _("Upper level:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextUpperLevel->Wrap( -1 );
	fgSizerUpperLevel->Add( m_staticTextUpperLevel, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_textCtrlUpperLevel = new ExtendedTextCtrl( sbSizerShoeHeight->GetStaticBox(), ID_UPPERLEVEL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerUpperLevel->Add( m_textCtrlUpperLevel, 0, wxALL|wxEXPAND, 5 );


	sbSizerShoeHeight->Add( fgSizerUpperLevel, 0, wxEXPAND, 5 );


	bSizerShoe->Add( sbSizerShoeHeight, 0, wxEXPAND, 5 );


	m_panelPageShoe->SetSizer( bSizerShoe );
	m_panelPageShoe->Layout();
	bSizerShoe->Fit( m_panelPageShoe );
	m_notebook->AddPage( m_panelPageShoe, _("Shoe"), false );
	m_panelPageSole = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer28;
	bSizer28 = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* sbSizerWalkcycleSupportHeel;
	sbSizerWalkcycleSupportHeel = new wxStaticBoxSizer( new wxStaticBox( m_panelPageSole, wxID_ANY, _("Walkcycle Support Heel") ), wxVERTICAL );

	m_sliderSupportHeelRadius = new wxSlider( sbSizerWalkcycleSupportHeel->GetStaticBox(), wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	sbSizerWalkcycleSupportHeel->Add( m_sliderSupportHeelRadius, 0, wxALL|wxEXPAND, 5 );

	m_sliderSupportHeelOffset = new wxSlider( sbSizerWalkcycleSupportHeel->GetStaticBox(), wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	sbSizerWalkcycleSupportHeel->Add( m_sliderSupportHeelOffset, 0, wxALL|wxEXPAND, 5 );


	bSizer28->Add( sbSizerWalkcycleSupportHeel, 0, wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizerWalkcycleSupportToe;
	sbSizerWalkcycleSupportToe = new wxStaticBoxSizer( new wxStaticBox( m_panelPageSole, wxID_ANY, _("Walkcycle Support Toe") ), wxVERTICAL );

	m_sliderSupportToeRadius = new wxSlider( sbSizerWalkcycleSupportToe->GetStaticBox(), ID_SLIDERTOESUPPORTRADIUS, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	sbSizerWalkcycleSupportToe->Add( m_sliderSupportToeRadius, 0, wxALL|wxEXPAND, 5 );

	m_sliderSupportToeOffset = new wxSlider( sbSizerWalkcycleSupportToe->GetStaticBox(), ID_SLIDERTOESUPPORTOFFSET, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	sbSizerWalkcycleSupportToe->Add( m_sliderSupportToeOffset, 0, wxALL|wxEXPAND, 5 );


	bSizer28->Add( sbSizerWalkcycleSupportToe, 0, wxEXPAND, 5 );

	m_staticTextConstruction = new wxStaticText( m_panelPageSole, wxID_ANY, _("Construction:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextConstruction->Wrap( -1 );
	bSizer28->Add( m_staticTextConstruction, 0, wxALL, 5 );

	m_choicebookConstruction = new wxChoicebook( m_panelPageSole, ID_GENERATOR, wxDefaultPosition, wxDefaultSize, wxCHB_DEFAULT );
	m_panelExperimental = new wxPanel( m_choicebookConstruction, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerExperimental;
	bSizerExperimental = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* sbSizerSole;
	sbSizerSole = new wxStaticBoxSizer( new wxStaticBox( m_panelExperimental, wxID_ANY, _("Sole") ), wxVERTICAL );

	m_choicebookHeelSeparation = new wxChoicebook( sbSizerSole->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxCHB_DEFAULT );
	m_panelHeelSeparated = new wxPanel( m_choicebookHeelSeparation, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerHeelSeparated;
	bSizerHeelSeparated = new wxBoxSizer( wxHORIZONTAL );


	bSizerHeelSeparated->Add( 0, 0, 1, wxEXPAND, 5 );

	m_buttonEditHeelShape = new wxButton( m_panelHeelSeparated, ID_EDITHEELSHAPE, _("Edit heel shape"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerHeelSeparated->Add( m_buttonEditHeelShape, 0, wxALL, 5 );

	m_buttonEditPlatformShape = new wxButton( m_panelHeelSeparated, ID_EDITPLATFORMSHAPE, _("Edit platform shape"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerHeelSeparated->Add( m_buttonEditPlatformShape, 0, wxALL, 5 );


	bSizerHeelSeparated->Add( 0, 0, 1, wxEXPAND, 5 );


	m_panelHeelSeparated->SetSizer( bSizerHeelSeparated );
	m_panelHeelSeparated->Layout();
	bSizerHeelSeparated->Fit( m_panelHeelSeparated );
	m_choicebookHeelSeparation->AddPage( m_panelHeelSeparated, _("Separated Heel & Platform"), false );
	m_panelSingleSole = new wxPanel( m_choicebookHeelSeparation, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerHeelJoined;
	bSizerHeelJoined = new wxBoxSizer( wxHORIZONTAL );


	bSizerHeelJoined->Add( 0, 0, 1, wxEXPAND, 5 );

	m_buttonEditSoleShape = new wxButton( m_panelSingleSole, ID_EDITSOLESHAPE, _("Edit sole shape"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerHeelJoined->Add( m_buttonEditSoleShape, 0, wxALL, 5 );


	bSizerHeelJoined->Add( 0, 0, 1, wxEXPAND, 5 );


	m_panelSingleSole->SetSizer( bSizerHeelJoined );
	m_panelSingleSole->Layout();
	bSizerHeelJoined->Fit( m_panelSingleSole );
	m_choicebookHeelSeparation->AddPage( m_panelSingleSole, _("Single Sole"), true );
	sbSizerSole->Add( m_choicebookHeelSeparation, 1, wxEXPAND | wxALL, 5 );


	bSizerExperimental->Add( sbSizerSole, 0, wxEXPAND, 5 );


	m_panelExperimental->SetSizer( bSizerExperimental );
	m_panelExperimental->Layout();
	bSizerExperimental->Fit( m_panelExperimental );
	m_choicebookConstruction->AddPage( m_panelExperimental, _("Experimental Generator"), false );
	m_panelWelted = new wxPanel( m_choicebookConstruction, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerWelted;
	bSizerWelted = new wxBoxSizer( wxVERTICAL );

	m_buttonEditHeelShape2 = new wxButton( m_panelWelted, ID_EDITHEELSHAPE, _("Edit heel shape"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerWelted->Add( m_buttonEditHeelShape2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_buttonEditWeltEdge = new wxButton( m_panelWelted, ID_EDITWELTEDGE, _("Edit welt edge"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerWelted->Add( m_buttonEditWeltEdge, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );


	m_panelWelted->SetSizer( bSizerWelted );
	m_panelWelted->Layout();
	bSizerWelted->Fit( m_panelWelted );
	m_choicebookConstruction->AddPage( m_panelWelted, _("Welted Construction"), true );
	m_panelCemented = new wxPanel( m_choicebookConstruction, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerCemented;
	bSizerCemented = new wxBoxSizer( wxVERTICAL );

	m_buttonEditSoleShape2 = new wxButton( m_panelCemented, ID_EDITSOLESHAPE, _("Edit sole shape"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerCemented->Add( m_buttonEditSoleShape2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );


	m_panelCemented->SetSizer( bSizerCemented );
	m_panelCemented->Layout();
	bSizerCemented->Fit( m_panelCemented );
	m_choicebookConstruction->AddPage( m_panelCemented, _("Cemented Sole"), false );
	m_panelMolded = new wxPanel( m_choicebookConstruction, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_choicebookConstruction->AddPage( m_panelMolded, _("Molded Sole"), false );
	m_panelClog = new wxPanel( m_choicebookConstruction, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerClog;
	bSizerClog = new wxBoxSizer( wxVERTICAL );

	m_buttonEditThickness = new wxButton( m_panelClog, ID_EDITTHICKNESS, _("Edit thickness"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerClog->Add( m_buttonEditThickness, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_buttonEditSoleShape3 = new wxButton( m_panelClog, ID_EDITSOLESHAPE, _("Edit sole shape"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerClog->Add( m_buttonEditSoleShape3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );


	m_panelClog->SetSizer( bSizerClog );
	m_panelClog->Layout();
	bSizerClog->Fit( m_panelClog );
	m_choicebookConstruction->AddPage( m_panelClog, _("Dutch Clog"), false );
	m_panelGeta = new wxPanel( m_choicebookConstruction, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerGeta;
	bSizerGeta = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizerGetaButtons;
	bSizerGetaButtons = new wxBoxSizer( wxHORIZONTAL );

	m_staticTextBridge = new wxStaticText( m_panelGeta, wxID_ANY, _("Bridge:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextBridge->Wrap( -1 );
	bSizerGetaButtons->Add( m_staticTextBridge, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_buttonAddBridge = new wxButton( m_panelGeta, ID_ADDBRIDGE, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerGetaButtons->Add( m_buttonAddBridge, 0, wxALL, 5 );

	m_buttonDeleteBridge = new wxButton( m_panelGeta, ID_DELETEBRIDGE, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerGetaButtons->Add( m_buttonDeleteBridge, 0, wxALL, 5 );


	bSizerGeta->Add( bSizerGetaButtons, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

	wxBoxSizer* bSizerGetaBridgeTable;
	bSizerGetaBridgeTable = new wxBoxSizer( wxHORIZONTAL );


	bSizerGetaBridgeTable->Add( 0, 0, 1, wxEXPAND, 5 );

	m_dataViewListCtrlBridges = new wxDataViewListCtrl( m_panelGeta, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_HORIZ_RULES|wxDV_ROW_LINES|wxDV_SINGLE|wxDV_VERT_RULES );
	m_dataViewListColumnBridgePosition = m_dataViewListCtrlBridges->AppendTextColumn( _("Position"), wxDATAVIEW_CELL_INERT, -1, static_cast<wxAlignment>(wxALIGN_LEFT), wxDATAVIEW_COL_RESIZABLE );
	m_dataViewListColumnBridgeWidth = m_dataViewListCtrlBridges->AppendTextColumn( _("Width"), wxDATAVIEW_CELL_INERT, -1, static_cast<wxAlignment>(wxALIGN_LEFT), wxDATAVIEW_COL_RESIZABLE );
	bSizerGetaBridgeTable->Add( m_dataViewListCtrlBridges, 2, wxALL|wxEXPAND, 5 );


	bSizerGetaBridgeTable->Add( 0, 0, 1, wxEXPAND, 5 );


	bSizerGeta->Add( bSizerGetaBridgeTable, 1, wxEXPAND, 5 );


	m_panelGeta->SetSizer( bSizerGeta );
	m_panelGeta->Layout();
	bSizerGeta->Fit( m_panelGeta );
	m_choicebookConstruction->AddPage( m_panelGeta, _("Geta"), false );
	bSizer28->Add( m_choicebookConstruction, 1, wxALL|wxEXPAND, 5 );


	m_panelPageSole->SetSizer( bSizer28 );
	m_panelPageSole->Layout();
	bSizer28->Fit( m_panelPageSole );
	m_notebook->AddPage( m_panelPageSole, _("Sole"), false );
	m_panelPagePattern = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxHORIZONTAL );

	m_treeListCtrlPattern = new wxTreeListCtrl( m_panelPagePattern, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTL_MULTIPLE );

	bSizer22->Add( m_treeListCtrlPattern, 1, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxVERTICAL );


	bSizer23->Add( 0, 0, 1, wxEXPAND, 5 );

	m_buttonOnPatternAddLine = new wxButton( m_panelPagePattern, ID_PATTERNADDLINE, _("Add Line"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer23->Add( m_buttonOnPatternAddLine, 0, wxALL|wxEXPAND, 5 );

	m_buttonOnPatternAddPatch = new wxButton( m_panelPagePattern, ID_PATTERNADDPATCH, _("Add Patch"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer23->Add( m_buttonOnPatternAddPatch, 0, wxALL|wxEXPAND, 5 );

	m_buttonOnPatternAddStitching = new wxButton( m_panelPagePattern, ID_PATTERNADDSTITCHING, _("Add Stitching"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer23->Add( m_buttonOnPatternAddStitching, 0, wxALL|wxEXPAND, 5 );

	m_buttonOnPatternAddPadding = new wxButton( m_panelPagePattern, ID_PATTERNADDPADDING, _("Add Padding"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer23->Add( m_buttonOnPatternAddPadding, 0, wxALL|wxEXPAND, 5 );

	m_buttonOnPatternAddCoordinateSystem = new wxButton( m_panelPagePattern, ID_PATTERNADDCOORDINATESYSTEM, _("Add Coordinate System"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer23->Add( m_buttonOnPatternAddCoordinateSystem, 0, wxALL|wxEXPAND, 5 );

	m_buttonOnPatternAddLaces = new wxButton( m_panelPagePattern, ID_PATTERNADDLACES, _("Add Laces"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer23->Add( m_buttonOnPatternAddLaces, 0, wxALL|wxEXPAND, 5 );

	m_buttonOnPatternAddAccessory = new wxButton( m_panelPagePattern, ID_PATTERNADDACCESSORY, _("Add Accessory"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer23->Add( m_buttonOnPatternAddAccessory, 0, wxALL|wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizer5;
	sbSizer5 = new wxStaticBoxSizer( new wxStaticBox( m_panelPagePattern, wxID_ANY, _("Fabrics") ), wxVERTICAL );

	m_listBoxFabric = new wxListBox( sbSizer5->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	m_listBoxFabric->Append( _("Leather - black") );
	m_listBoxFabric->Append( _("Leather - red") );
	m_listBoxFabric->Append( _("Leather - white") );
	m_listBoxFabric->Append( _("Patent - black") );
	m_listBoxFabric->Append( _("Patent - white") );
	m_listBoxFabric->Append( _("Cotton - black") );
	m_listBoxFabric->Append( _("Cotton - yellow") );
	m_listBoxFabric->Append( _("Cotton - red") );
	sbSizer5->Add( m_listBoxFabric, 2, wxALL|wxEXPAND, 5 );


	bSizer23->Add( sbSizer5, 1, wxEXPAND, 5 );


	bSizer22->Add( bSizer23, 1, wxEXPAND, 5 );


	m_panelPagePattern->SetSizer( bSizer22 );
	m_panelPagePattern->Layout();
	bSizer22->Fit( m_panelPagePattern );
	m_notebook->AddPage( m_panelPagePattern, _("Pattern"), false );
	m_panelPageFlattening = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer241;
	bSizer241 = new wxBoxSizer( wxHORIZONTAL );


	bSizer241->Add( 0, 0, 1, 0, 5 );

	wxBoxSizer* bSizer25;
	bSizer25 = new wxBoxSizer( wxVERTICAL );

	m_buttonExportFlattening = new wxButton( m_panelPageFlattening, ID_EXPORTFLATTENING, _("Export Flattening"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer25->Add( m_buttonExportFlattening, 0, wxALL|wxEXPAND, 5 );

	m_buttonTestStitch = new wxButton( m_panelPageFlattening, ID_TESTSTITCH, _("Test Stitch"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer25->Add( m_buttonTestStitch, 0, wxALL|wxEXPAND, 5 );


	bSizer241->Add( bSizer25, 0, 0, 5 );


	m_panelPageFlattening->SetSizer( bSizer241 );
	m_panelPageFlattening->Layout();
	bSizer241->Fit( m_panelPageFlattening );
	m_notebook->AddPage( m_panelPageFlattening, _("Flattening"), false );

	bSizerConfig->Add( m_notebook, 1, wxEXPAND | wxALL, 5 );


	m_panelConfig->SetSizer( bSizerConfig );
	m_panelConfig->Layout();
	bSizerConfig->Fit( m_panelConfig );
	m_panelCanvas = new wxPanel( m_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerCanvas;
	bSizerCanvas = new wxBoxSizer( wxVERTICAL );

	m_canvas3D = new Canvas3D(m_panelCanvas);
	bSizerCanvas->Add( m_canvas3D, 1, wxALL|wxEXPAND, 5 );

	m_panelPattern = new PanelPattern(m_panelCanvas);
	m_panelPattern->Hide();

	bSizerCanvas->Add( m_panelPattern, 1, wxALL|wxEXPAND, 5 );

	m_panelCheck = new wxPanel( m_panelCanvas, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panelCheck->Hide();

	wxBoxSizer* bSizerCheckRounding;
	bSizerCheckRounding = new wxBoxSizer( wxHORIZONTAL );

	m_panelSupport = new PanelSupport(m_panelCheck);
	bSizerCheckRounding->Add( m_panelSupport, 1, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizerSupport;
	bSizerSupport = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* sbSizerCycle;
	sbSizerCycle = new wxStaticBoxSizer( new wxStaticBox( m_panelCheck, wxID_ANY, _("Walkcycle") ), wxVERTICAL );

	m_panelCycle = new PanelWalkcycle(m_panelCheck);
	sbSizerCycle->Add( m_panelCycle, 1, wxALL|wxEXPAND, 5 );

	m_checkBoxLockAnkle = new wxCheckBox( sbSizerCycle->GetStaticBox(), wxID_ANY, _("Lock Ankle"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizerCycle->Add( m_checkBoxLockAnkle, 0, wxALL|wxEXPAND, 5 );


	bSizerSupport->Add( sbSizerCycle, 1, wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizerPlot;
	sbSizerPlot = new wxStaticBoxSizer( new wxStaticBox( m_panelCheck, wxID_ANY, _("Forces, Moments") ), wxVERTICAL );

	wxString m_choiceDisplayChoices[] = { _("Kneeline"), _("Force"), _("Moment"), _("Crossforce") };
	int m_choiceDisplayNChoices = sizeof( m_choiceDisplayChoices ) / sizeof( wxString );
	m_choiceDisplay = new wxChoice( sbSizerPlot->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceDisplayNChoices, m_choiceDisplayChoices, 0 );
	m_choiceDisplay->SetSelection( 0 );
	sbSizerPlot->Add( m_choiceDisplay, 0, wxALL|wxEXPAND, 5 );

	m_panelPlot = new PanelPlotSimple(m_panelCheck);
	sbSizerPlot->Add( m_panelPlot, 1, wxALL|wxEXPAND, 5 );


	bSizerSupport->Add( sbSizerPlot, 1, wxEXPAND, 5 );


	bSizerCheckRounding->Add( bSizerSupport, 0, wxEXPAND, 5 );


	m_panelCheck->SetSizer( bSizerCheckRounding );
	m_panelCheck->Layout();
	bSizerCheckRounding->Fit( m_panelCheck );
	bSizerCanvas->Add( m_panelCheck, 1, wxEXPAND | wxALL, 5 );


	m_panelCanvas->SetSizer( bSizerCanvas );
	m_panelCanvas->Layout();
	bSizerCanvas->Fit( m_panelCanvas );
	m_splitter->SplitVertically( m_panelConfig, m_panelCanvas, 550 );
	bSizer->Add( m_splitter, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer );
	this->Layout();
	m_statusBar = this->CreateStatusBar( 2, wxSTB_SIZEGRIP, wxID_ANY );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GUIFrameMain::OnClose ) );
	m_menuFoot->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnQuickSetupMeasurements ), this, m_menuItemQuickSetup->GetId());
	m_menuFootParameters->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnChangeModel ), this, m_menuItemFootModelBone->GetId());
	m_menuFootParameters->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnChangeModel ), this, m_menuItemFootModelScan->GetId());
	m_menuFoot->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnChangeModel ), this, m_menuItemSymmetry->GetId());
	m_menuMeasurementsCopy->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnCopyMeasurements ), this, m_menuItemMeasurementsCopyActiveToInactive->GetId());
	m_menuMeasurementsCopy->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnCopyMeasurements ), this, m_menuItemMeasurementsCopyRightToLeft->GetId());
	m_menuMeasurementsCopy->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnCopyMeasurements ), this, m_menuItemMeasurementsCopyLeftToRight->GetId());
	m_menuFoot->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnLoadFootSTL ), this, m_menuItemFootModelLoadSTL->GetId());
	m_menuFootModel->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnChangeModel ), this, m_menuItemBoneBasedModel->GetId());
	m_menuFootModel->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnChangeModel ), this, m_menuItemUseLastBasedModel->GetId());
	m_menuBoneModel->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnEditBoneModel ), this, m_menuItemEditBoneModel->GetId());
	m_menuBoneModel->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnLoadBoneModel ), this, m_menuItemLoadBoneModel->GetId());
	m_menuBoneModel->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnSaveBoneModel ), this, m_menuItemSaveBoneModel->GetId());
	m_menuConstruction->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnConstructionSelection ), this, m_menuItemTestGenerator->GetId());
	m_menuConstruction->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnConstructionSelection ), this, m_menuItemConstructionClassic->GetId());
	m_menuConstruction->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnConstructionSelection ), this, m_menuItemConstructionCemented->GetId());
	m_menuConstruction->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnConstructionSelection ), this, m_menuItemConstructionMolded->GetId());
	m_menuConstruction->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnConstructionSelection ), this, m_menuItemConstructionDutch->GetId());
	m_menuConstruction->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnConstructionSelection ), this, m_menuItemConstructionGeta->GetId());
	m_menuPattern->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnLoadPattern ), this, m_menuItemLoadPattern->GetId());
	m_menuPattern->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnSavePattern ), this, m_menuItemSavePattern->GetId());
	m_menuGeometry->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnSaveLast ), this, m_menuItemSaveLast->GetId());
	m_menuGeometry->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnSaveInsole ), this, m_menuItemSaveInsole->GetId());
	m_menuGeometry->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnSaveSole ), this, m_menuItemSaveSole->GetId());
	m_menuGeometry->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnSaveCutaway ), this, m_menuItemCutaway->GetId());
	m_menuGeometry->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnPackZip ), this, m_menuItemPackZip->GetId());
	m_menuView->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnToggleStereo3D ), this, m_menuItemStereo3D->GetId());
	m_menuView->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnViewChange ), this, m_menuItemShowLeft->GetId());
	m_menuView->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnViewChange ), this, m_menuItemShowRight->GetId());
	m_menuView->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnViewChange ), this, m_menuItemShowBones->GetId());
	m_menuView->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnViewChange ), this, m_menuItemShowSkin->GetId());
	m_menuView->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnViewChange ), this, m_menuItemShowLeg->GetId());
	m_menuView->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnViewChange ), this, m_menuItemShowLast->GetId());
	m_menuView->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnViewChange ), this, m_menuItemShowInsole->GetId());
	m_menuView->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnViewChange ), this, m_menuIItemShowSole->GetId());
	m_menuView->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnViewChange ), this, m_menuItemShowUpper->GetId());
	m_menuView->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnViewChange ), this, m_menuItemShowCutaway->GetId());
	m_menuView->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnViewChange ), this, m_menuItemShowFloor->GetId());
	m_menuView->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnViewChange ), this, m_menuItemShowCoordinateSystem->GetId());
	m_menuView->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnViewChange ), this, m_menuItemShowBackground->GetId());
	m_menuView->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnSetupBackgroundImages ), this, m_menuItemSetupBackground->GetId());
	m_menuHelp->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnDebugParser ), this, m_menuItemdebugParser->GetId());
	m_panelPageFoot->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_toggleBtnEditLeft->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnToggleButton ), NULL, this );
	m_toggleBtnEditRight->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnToggleButton ), NULL, this );
	m_choicebookMeasurement->Connect( wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGED, wxChoicebookEventHandler( GUIFrameMain::OnPageChange ), NULL, this );
	m_textCtrlFootLength->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlFootLength->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlFootLength->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlFootLength->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlBallWidth->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlBallWidth->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlBallWidth->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlBallWidth->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlBigToeGirth->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlBigToeGirth->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlBigToeGirth->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlBigToeGirth->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlLittleToeGirth->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlLittleToeGirth->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlLittleToeGirth->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlLittleToeGirth->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlWaistGirth->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlWaistGirth->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlWaistGirth->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlWaistGirth->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlHeelGirth->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlHeelGirth->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlHeelGirth->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlHeelGirth->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlHeelWidth->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlHeelWidth->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlHeelWidth->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlHeelWidth->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlAngleMixing->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlAngleMixing->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlAngleMixing->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlAngleMixing->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlLegLengthDifference->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlLegLengthDifference->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlLegLengthDifference->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlLegLengthDifference->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_buttonQuickSetup->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnQuickSetupMeasurements ), NULL, this );
	m_bitmapFoot->Connect( wxEVT_SIZE, wxSizeEventHandler( GUIFrameMain::OnSize ), NULL, this );
	m_filePickerScanFile->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( GUIFrameMain::OnFileChangedScanFile ), NULL, this );
	m_choicebookFootModel->Connect( wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGED, wxChoicebookEventHandler( GUIFrameMain::OnPageChange ), NULL, this );
	m_buttonEditBoneModel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnEditBoneModel ), NULL, this );
	m_filePickerLastModel->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( GUIFrameMain::OnFileChangedLastFile ), NULL, this );
	m_toggleBtnEditLeft1->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnToggleButton ), NULL, this );
	m_toggleBtnEditRight1->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnToggleButton ), NULL, this );
	m_textCtrlBelowCrutchGirth->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlBelowCrutchGirth->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlBelowCrutchGirth->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlBelowCrutchGirth->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlBelowCrutchLevel->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlBelowCrutchLevel->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlBelowCrutchLevel->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlBelowCrutchLevel->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlMiddleOfCalfGirth->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlMiddleOfCalfGirth->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlMiddleOfCalfGirth->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlMiddleOfCalfGirth->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlMiddleOfCalfLevel->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlMiddleOfCalfLevel->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlMiddleOfCalfLevel->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlMiddleOfCalfLevel->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlAboveKneeGirth->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlAboveKneeGirth->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlAboveKneeGirth->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlAboveKneeGirth->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlAboveKneeLevel->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlAboveKneeLevel->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlAboveKneeLevel->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlAboveKneeLevel->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlOverKneeCapGirth->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlOverKneeCapGirth->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlOverKneeCapGirth->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlOverKneeCapGirth->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlOverKneeCapLevel->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlOverKneeCapLevel->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlOverKneeCapLevel->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlOverKneeCapLevel->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlBelowKneeGirth->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlBelowKneeGirth->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlBelowKneeGirth->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlBelowKneeGirth->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlBelowKneeLevel->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlBelowKneeLevel->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlBelowKneeLevel->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlBelowKneeLevel->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlMiddleOfShankGirth->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlMiddleOfShankGirth->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlMiddleOfShankGirth->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlMiddleOfShankGirth->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlMiddleOfShankLevel->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlMiddleOfShankLevel->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlMiddleOfShankLevel->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlMiddleOfShankLevel->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlAboveAnkleGirth->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlAboveAnkleGirth->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlAboveAnkleGirth->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlAboveAnkleGirth->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlAboveAnkleLevel->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlAboveAnkleLevel->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlAboveAnkleLevel->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlAboveAnkleLevel->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlOverAnkleBoneGirth->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlOverAnkleBoneGirth->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlOverAnkleBoneGirth->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlOverAnkleBoneGirth->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlOverAnkleBoneLevel->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlOverAnkleBoneLevel->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlOverAnkleBoneLevel->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlOverAnkleBoneLevel->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlBigToeAngle->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlBigToeAngle->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlBigToeAngle->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlBigToeAngle->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlLittleToeAngle->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlLittleToeAngle->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlLittleToeAngle->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlLittleToeAngle->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlBallMeasurementAngle->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlBallMeasurementAngle->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlBallMeasurementAngle->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlBallMeasurementAngle->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlHeelDirectionAngle->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlHeelDirectionAngle->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlHeelDirectionAngle->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlHeelDirectionAngle->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_sliderTipSharpness->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderTipSharpness->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderTipSharpness->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderTipSharpness->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderTipSharpness->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderTipSharpness->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderTipSharpness->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderTipSharpness->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderTipSharpness->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_textCtrlExtraLength->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlExtraLength->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlExtraLength->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlExtraLength->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlFootCompression->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlFootCompression->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlFootCompression->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlFootCompression->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_choiceShoeType->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GUIFrameMain::OnChoice ), NULL, this );
	m_textCtrlHeelHeight->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlHeelHeight->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlHeelHeight->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlHeelHeight->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlBallHeight->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlBallHeight->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlBallHeight->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlBallHeight->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlHeelPitch->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlHeelPitch->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlHeelPitch->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlHeelPitch->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlToeSpring->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlToeSpring->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlToeSpring->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlToeSpring->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_choiceShoeHeight->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GUIFrameMain::OnChoice ), NULL, this );
	m_textCtrlUpperLevel->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlUpperLevel->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlUpperLevel->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlUpperLevel->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_sliderSupportHeelRadius->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportHeelRadius->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportHeelRadius->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportHeelRadius->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportHeelRadius->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportHeelRadius->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportHeelRadius->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportHeelRadius->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportHeelRadius->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportHeelOffset->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportHeelOffset->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportHeelOffset->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportHeelOffset->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportHeelOffset->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportHeelOffset->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportHeelOffset->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportHeelOffset->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportHeelOffset->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportToeRadius->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportToeRadius->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportToeRadius->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportToeRadius->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportToeRadius->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportToeRadius->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportToeRadius->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportToeRadius->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportToeRadius->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportToeOffset->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportToeOffset->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportToeOffset->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportToeOffset->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportToeOffset->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportToeOffset->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportToeOffset->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportToeOffset->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportToeOffset->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_buttonEditHeelShape->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnEditShape ), NULL, this );
	m_buttonEditPlatformShape->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnEditShape ), NULL, this );
	m_buttonEditSoleShape->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnEditShape ), NULL, this );
	m_buttonEditHeelShape2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnEditShape ), NULL, this );
	m_buttonEditWeltEdge->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnEditShape ), NULL, this );
	m_buttonEditSoleShape2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnEditShape ), NULL, this );
	m_buttonEditThickness->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnEditShape ), NULL, this );
	m_buttonEditSoleShape3->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnEditShape ), NULL, this );
	m_buttonAddBridge->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnAddBridge ), NULL, this );
	m_buttonDeleteBridge->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnDeleteBridge ), NULL, this );
	m_dataViewListCtrlBridges->Connect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( GUIFrameMain::OnListCtrlOnSelectionChanged ), NULL, this );
	m_treeListCtrlPattern->Connect( wxEVT_TREELIST_SELECTION_CHANGED, wxTreeListEventHandler( GUIFrameMain::OnPatternSelect ), NULL, this );
	m_buttonOnPatternAddLine->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnPatternAdd ), NULL, this );
	m_buttonOnPatternAddPatch->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnPatternAdd ), NULL, this );
	m_buttonOnPatternAddStitching->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnPatternAdd ), NULL, this );
	m_buttonOnPatternAddPadding->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnPatternAdd ), NULL, this );
	m_buttonOnPatternAddCoordinateSystem->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnPatternAdd ), NULL, this );
	m_buttonOnPatternAddLaces->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnPatternAdd ), NULL, this );
	m_buttonOnPatternAddAccessory->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnPatternAdd ), NULL, this );
	m_listBoxFabric->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( GUIFrameMain::OnPatternSelectFabric ), NULL, this );
	m_canvas3D->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( GUIFrameMain::On3DSelect ), NULL, this );
	m_checkBoxLockAnkle->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GUIFrameMain::OnToggleAnkleLock ), NULL, this );
	m_choiceDisplay->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GUIFrameMain::OnChoiceDisplay ), NULL, this );
}

GUIFrameMain::~GUIFrameMain()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GUIFrameMain::OnClose ) );
	m_panelPageFoot->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_toggleBtnEditLeft->Disconnect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnToggleButton ), NULL, this );
	m_toggleBtnEditRight->Disconnect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnToggleButton ), NULL, this );
	m_choicebookMeasurement->Disconnect( wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGED, wxChoicebookEventHandler( GUIFrameMain::OnPageChange ), NULL, this );
	m_textCtrlFootLength->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlFootLength->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlFootLength->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlFootLength->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlBallWidth->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlBallWidth->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlBallWidth->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlBallWidth->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlBigToeGirth->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlBigToeGirth->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlBigToeGirth->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlBigToeGirth->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlLittleToeGirth->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlLittleToeGirth->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlLittleToeGirth->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlLittleToeGirth->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlWaistGirth->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlWaistGirth->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlWaistGirth->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlWaistGirth->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlHeelGirth->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlHeelGirth->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlHeelGirth->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlHeelGirth->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlHeelWidth->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlHeelWidth->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlHeelWidth->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlHeelWidth->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlAngleMixing->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlAngleMixing->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlAngleMixing->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlAngleMixing->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlLegLengthDifference->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlLegLengthDifference->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlLegLengthDifference->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlLegLengthDifference->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_buttonQuickSetup->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnQuickSetupMeasurements ), NULL, this );
	m_bitmapFoot->Disconnect( wxEVT_SIZE, wxSizeEventHandler( GUIFrameMain::OnSize ), NULL, this );
	m_filePickerScanFile->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( GUIFrameMain::OnFileChangedScanFile ), NULL, this );
	m_choicebookFootModel->Disconnect( wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGED, wxChoicebookEventHandler( GUIFrameMain::OnPageChange ), NULL, this );
	m_buttonEditBoneModel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnEditBoneModel ), NULL, this );
	m_filePickerLastModel->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( GUIFrameMain::OnFileChangedLastFile ), NULL, this );
	m_toggleBtnEditLeft1->Disconnect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnToggleButton ), NULL, this );
	m_toggleBtnEditRight1->Disconnect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnToggleButton ), NULL, this );
	m_textCtrlBelowCrutchGirth->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlBelowCrutchGirth->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlBelowCrutchGirth->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlBelowCrutchGirth->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlBelowCrutchLevel->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlBelowCrutchLevel->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlBelowCrutchLevel->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlBelowCrutchLevel->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlMiddleOfCalfGirth->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlMiddleOfCalfGirth->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlMiddleOfCalfGirth->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlMiddleOfCalfGirth->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlMiddleOfCalfLevel->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlMiddleOfCalfLevel->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlMiddleOfCalfLevel->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlMiddleOfCalfLevel->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlAboveKneeGirth->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlAboveKneeGirth->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlAboveKneeGirth->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlAboveKneeGirth->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlAboveKneeLevel->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlAboveKneeLevel->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlAboveKneeLevel->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlAboveKneeLevel->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlOverKneeCapGirth->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlOverKneeCapGirth->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlOverKneeCapGirth->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlOverKneeCapGirth->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlOverKneeCapLevel->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlOverKneeCapLevel->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlOverKneeCapLevel->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlOverKneeCapLevel->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlBelowKneeGirth->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlBelowKneeGirth->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlBelowKneeGirth->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlBelowKneeGirth->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlBelowKneeLevel->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlBelowKneeLevel->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlBelowKneeLevel->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlBelowKneeLevel->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlMiddleOfShankGirth->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlMiddleOfShankGirth->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlMiddleOfShankGirth->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlMiddleOfShankGirth->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlMiddleOfShankLevel->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlMiddleOfShankLevel->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlMiddleOfShankLevel->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlMiddleOfShankLevel->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlAboveAnkleGirth->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlAboveAnkleGirth->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlAboveAnkleGirth->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlAboveAnkleGirth->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlAboveAnkleLevel->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlAboveAnkleLevel->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlAboveAnkleLevel->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlAboveAnkleLevel->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlOverAnkleBoneGirth->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlOverAnkleBoneGirth->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlOverAnkleBoneGirth->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlOverAnkleBoneGirth->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlOverAnkleBoneLevel->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlOverAnkleBoneLevel->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlOverAnkleBoneLevel->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlOverAnkleBoneLevel->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlBigToeAngle->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlBigToeAngle->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlBigToeAngle->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlBigToeAngle->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlLittleToeAngle->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlLittleToeAngle->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlLittleToeAngle->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlLittleToeAngle->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlBallMeasurementAngle->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlBallMeasurementAngle->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlBallMeasurementAngle->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlBallMeasurementAngle->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlHeelDirectionAngle->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlHeelDirectionAngle->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlHeelDirectionAngle->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlHeelDirectionAngle->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_sliderTipSharpness->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderTipSharpness->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderTipSharpness->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderTipSharpness->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderTipSharpness->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderTipSharpness->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderTipSharpness->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderTipSharpness->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderTipSharpness->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_textCtrlExtraLength->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlExtraLength->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlExtraLength->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlExtraLength->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlFootCompression->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlFootCompression->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlFootCompression->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlFootCompression->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_choiceShoeType->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GUIFrameMain::OnChoice ), NULL, this );
	m_textCtrlHeelHeight->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlHeelHeight->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlHeelHeight->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlHeelHeight->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlBallHeight->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlBallHeight->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlBallHeight->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlBallHeight->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlHeelPitch->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlHeelPitch->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlHeelPitch->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlHeelPitch->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlToeSpring->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlToeSpring->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlToeSpring->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlToeSpring->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_choiceShoeHeight->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GUIFrameMain::OnChoice ), NULL, this );
	m_textCtrlUpperLevel->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlUpperLevel->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlUpperLevel->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlUpperLevel->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_sliderSupportHeelRadius->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportHeelRadius->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportHeelRadius->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportHeelRadius->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportHeelRadius->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportHeelRadius->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportHeelRadius->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportHeelRadius->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportHeelRadius->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportHeelOffset->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportHeelOffset->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportHeelOffset->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportHeelOffset->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportHeelOffset->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportHeelOffset->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportHeelOffset->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportHeelOffset->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportHeelOffset->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportToeRadius->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportToeRadius->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportToeRadius->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportToeRadius->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportToeRadius->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportToeRadius->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportToeRadius->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportToeRadius->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportToeRadius->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportToeOffset->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportToeOffset->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportToeOffset->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportToeOffset->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportToeOffset->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportToeOffset->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportToeOffset->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportToeOffset->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSupportToeOffset->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_buttonEditHeelShape->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnEditShape ), NULL, this );
	m_buttonEditPlatformShape->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnEditShape ), NULL, this );
	m_buttonEditSoleShape->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnEditShape ), NULL, this );
	m_buttonEditHeelShape2->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnEditShape ), NULL, this );
	m_buttonEditWeltEdge->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnEditShape ), NULL, this );
	m_buttonEditSoleShape2->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnEditShape ), NULL, this );
	m_buttonEditThickness->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnEditShape ), NULL, this );
	m_buttonEditSoleShape3->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnEditShape ), NULL, this );
	m_buttonAddBridge->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnAddBridge ), NULL, this );
	m_buttonDeleteBridge->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnDeleteBridge ), NULL, this );
	m_dataViewListCtrlBridges->Disconnect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( GUIFrameMain::OnListCtrlOnSelectionChanged ), NULL, this );
	m_treeListCtrlPattern->Disconnect( wxEVT_TREELIST_SELECTION_CHANGED, wxTreeListEventHandler( GUIFrameMain::OnPatternSelect ), NULL, this );
	m_buttonOnPatternAddLine->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnPatternAdd ), NULL, this );
	m_buttonOnPatternAddPatch->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnPatternAdd ), NULL, this );
	m_buttonOnPatternAddStitching->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnPatternAdd ), NULL, this );
	m_buttonOnPatternAddPadding->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnPatternAdd ), NULL, this );
	m_buttonOnPatternAddCoordinateSystem->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnPatternAdd ), NULL, this );
	m_buttonOnPatternAddLaces->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnPatternAdd ), NULL, this );
	m_buttonOnPatternAddAccessory->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnPatternAdd ), NULL, this );
	m_listBoxFabric->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( GUIFrameMain::OnPatternSelectFabric ), NULL, this );
	m_canvas3D->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( GUIFrameMain::On3DSelect ), NULL, this );
	m_checkBoxLockAnkle->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GUIFrameMain::OnToggleAnkleLock ), NULL, this );
	m_choiceDisplay->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GUIFrameMain::OnChoiceDisplay ), NULL, this );

}

GUIFrameBoneModel::GUIFrameBoneModel( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer;
	bSizer = new wxBoxSizer( wxVERTICAL );

	m_notebook = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_MULTILINE );
	m_panelBone = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerLength;
	bSizerLength = new wxBoxSizer( wxVERTICAL );

	m_gridBoneLength = new wxGrid( m_panelBone, ID_GRIDLENGTH, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	m_gridBoneLength->CreateGrid( 0, 2 );
	m_gridBoneLength->EnableEditing( true );
	m_gridBoneLength->EnableGridLines( true );
	m_gridBoneLength->EnableDragGridSize( false );
	m_gridBoneLength->SetMargins( 0, 0 );

	// Columns
	m_gridBoneLength->SetColSize( 0, 130 );
	m_gridBoneLength->SetColSize( 1, 112 );
	m_gridBoneLength->EnableDragColMove( false );
	m_gridBoneLength->EnableDragColSize( true );
	m_gridBoneLength->SetColLabelValue( 0, _("Length (Formula)") );
	m_gridBoneLength->SetColLabelValue( 1, _("Length (Value)") );
	m_gridBoneLength->SetColLabelSize( 30 );
	m_gridBoneLength->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	m_gridBoneLength->AutoSizeRows();
	m_gridBoneLength->EnableDragRowSize( true );
	m_gridBoneLength->SetRowLabelSize( 100 );
	m_gridBoneLength->SetRowLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Label Appearance

	// Cell Defaults
	m_gridBoneLength->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	bSizerLength->Add( m_gridBoneLength, 1, wxALL|wxEXPAND, 5 );


	m_panelBone->SetSizer( bSizerLength );
	m_panelBone->Layout();
	bSizerLength->Fit( m_panelBone );
	m_notebook->AddPage( m_panelBone, _("Bone Length"), true );
	m_panelBoneDiameter = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerDiameter;
	bSizerDiameter = new wxBoxSizer( wxVERTICAL );

	m_gridBoneDiameter = new wxGrid( m_panelBoneDiameter, ID_GRIDDIAMETER, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	m_gridBoneDiameter->CreateGrid( 0, 4 );
	m_gridBoneDiameter->EnableEditing( true );
	m_gridBoneDiameter->EnableGridLines( true );
	m_gridBoneDiameter->EnableDragGridSize( false );
	m_gridBoneDiameter->SetMargins( 0, 0 );

	// Columns
	m_gridBoneDiameter->SetColSize( 0, 100 );
	m_gridBoneDiameter->SetColSize( 1, 100 );
	m_gridBoneDiameter->SetColSize( 2, 100 );
	m_gridBoneDiameter->SetColSize( 3, 100 );
	m_gridBoneDiameter->EnableDragColMove( false );
	m_gridBoneDiameter->EnableDragColSize( true );
	m_gridBoneDiameter->SetColLabelValue( 0, _("r1 (Formula)") );
	m_gridBoneDiameter->SetColLabelValue( 1, _("r1 (Value)") );
	m_gridBoneDiameter->SetColLabelValue( 2, _("r2 (Formula)") );
	m_gridBoneDiameter->SetColLabelValue( 3, _("r2 (Value)") );
	m_gridBoneDiameter->SetColLabelSize( 30 );
	m_gridBoneDiameter->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	m_gridBoneDiameter->EnableDragRowSize( true );
	m_gridBoneDiameter->SetRowLabelSize( 100 );
	m_gridBoneDiameter->SetRowLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Label Appearance

	// Cell Defaults
	m_gridBoneDiameter->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	bSizerDiameter->Add( m_gridBoneDiameter, 1, wxALL|wxEXPAND, 5 );


	m_panelBoneDiameter->SetSizer( bSizerDiameter );
	m_panelBoneDiameter->Layout();
	bSizerDiameter->Fit( m_panelBoneDiameter );
	m_notebook->AddPage( m_panelBoneDiameter, _("Bone Diameter"), false );
	m_panelSkinThickness = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerSkinThickness;
	bSizerSkinThickness = new wxBoxSizer( wxVERTICAL );

	m_gridSkinThickness = new wxGrid( m_panelSkinThickness, ID_GRIDSKIN, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	m_gridSkinThickness->CreateGrid( 0, 4 );
	m_gridSkinThickness->EnableEditing( true );
	m_gridSkinThickness->EnableGridLines( true );
	m_gridSkinThickness->EnableDragGridSize( false );
	m_gridSkinThickness->SetMargins( 0, 0 );

	// Columns
	m_gridSkinThickness->SetColSize( 0, 100 );
	m_gridSkinThickness->SetColSize( 1, 100 );
	m_gridSkinThickness->SetColSize( 2, 100 );
	m_gridSkinThickness->SetColSize( 3, 100 );
	m_gridSkinThickness->EnableDragColMove( false );
	m_gridSkinThickness->EnableDragColSize( true );
	m_gridSkinThickness->SetColLabelValue( 0, _("s1 (Formula)") );
	m_gridSkinThickness->SetColLabelValue( 1, _("s1 (Value)") );
	m_gridSkinThickness->SetColLabelValue( 2, _("s2 (Formula)") );
	m_gridSkinThickness->SetColLabelValue( 3, _("s2 (Value)") );
	m_gridSkinThickness->SetColLabelSize( 30 );
	m_gridSkinThickness->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	m_gridSkinThickness->EnableDragRowSize( true );
	m_gridSkinThickness->SetRowLabelSize( 100 );
	m_gridSkinThickness->SetRowLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Label Appearance

	// Cell Defaults
	m_gridSkinThickness->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	bSizerSkinThickness->Add( m_gridSkinThickness, 1, wxALL|wxEXPAND, 5 );


	m_panelSkinThickness->SetSizer( bSizerSkinThickness );
	m_panelSkinThickness->Layout();
	bSizerSkinThickness->Fit( m_panelSkinThickness );
	m_notebook->AddPage( m_panelSkinThickness, _("Skin Thickness"), false );

	bSizer->Add( m_notebook, 1, wxEXPAND | wxALL, 5 );


	this->SetSizer( bSizer );
	this->Layout();
	m_menubar = new wxMenuBar( 0 );
	m_menuFile = new wxMenu();
	wxMenuItem* m_menuItemReset;
	m_menuItemReset = new wxMenuItem( m_menuFile, wxID_NEW, wxString( _("&Reset") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( m_menuItemReset );

	wxMenuItem* m_menuItemLoad;
	m_menuItemLoad = new wxMenuItem( m_menuFile, wxID_OPEN, wxString( _("&Load ...") ) + wxT('\t') + wxT("Ctrl+L"), wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( m_menuItemLoad );

	wxMenuItem* m_menuItemSave;
	m_menuItemSave = new wxMenuItem( m_menuFile, wxID_SAVE, wxString( _("&Save") ) + wxT('\t') + wxT("Ctrl+S"), wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( m_menuItemSave );

	wxMenuItem* m_menuItemSaveAs;
	m_menuItemSaveAs = new wxMenuItem( m_menuFile, wxID_SAVEAS, wxString( _("Save &as ...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( m_menuItemSaveAs );

	m_menuFile->AppendSeparator();

	wxMenuItem* m_menuItemClose;
	m_menuItemClose = new wxMenuItem( m_menuFile, wxID_CLOSE, wxString( _("&Close Window") ) + wxT('\t') + wxT("Ctrl+W"), wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( m_menuItemClose );

	m_menubar->Append( m_menuFile, _("&File") );

	m_menuHelp = new wxMenu();
	wxMenuItem* m_menuItemHelp;
	m_menuItemHelp = new wxMenuItem( m_menuHelp, wxID_HELP, wxString( _("&Help") ) + wxT('\t') + wxT("F1"), wxEmptyString, wxITEM_NORMAL );
	m_menuHelp->Append( m_menuItemHelp );

	m_menubar->Append( m_menuHelp, _("&Help") );

	this->SetMenuBar( m_menubar );


	this->Centre( wxBOTH );

	// Connect Events
	m_gridBoneLength->Connect( wxEVT_GRID_CELL_CHANGED, wxGridEventHandler( GUIFrameBoneModel::OnCellChange ), NULL, this );
	m_gridBoneDiameter->Connect( wxEVT_GRID_CELL_CHANGED, wxGridEventHandler( GUIFrameBoneModel::OnCellChange ), NULL, this );
	m_gridSkinThickness->Connect( wxEVT_GRID_CELL_CHANGED, wxGridEventHandler( GUIFrameBoneModel::OnCellChange ), NULL, this );
}

GUIFrameBoneModel::~GUIFrameBoneModel()
{
	// Disconnect Events
	m_gridBoneLength->Disconnect( wxEVT_GRID_CELL_CHANGED, wxGridEventHandler( GUIFrameBoneModel::OnCellChange ), NULL, this );
	m_gridBoneDiameter->Disconnect( wxEVT_GRID_CELL_CHANGED, wxGridEventHandler( GUIFrameBoneModel::OnCellChange ), NULL, this );
	m_gridSkinThickness->Disconnect( wxEVT_GRID_CELL_CHANGED, wxGridEventHandler( GUIFrameBoneModel::OnCellChange ), NULL, this );

}

GUIFrameEditorFootModel::GUIFrameEditorFootModel( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer;
	bSizer = new wxBoxSizer( wxHORIZONTAL );

	m_canvasFootModel = new CanvasFootModel(this);
	bSizer->Add( m_canvasFootModel, 2, wxALL|wxEXPAND, 5 );

	m_dataViewListCtrl2 = new wxDataViewListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_dataViewListColumn4 = m_dataViewListCtrl2->AppendTextColumn( _("Bone"), wxDATAVIEW_CELL_INERT, -1, static_cast<wxAlignment>(wxALIGN_LEFT), wxDATAVIEW_COL_RESIZABLE );
	m_dataViewListColumn5 = m_dataViewListCtrl2->AppendTextColumn( _("S1"), wxDATAVIEW_CELL_INERT, -1, static_cast<wxAlignment>(wxALIGN_LEFT), wxDATAVIEW_COL_RESIZABLE );
	m_dataViewListColumn6 = m_dataViewListCtrl2->AppendTextColumn( _("S2"), wxDATAVIEW_CELL_INERT, -1, static_cast<wxAlignment>(wxALIGN_LEFT), wxDATAVIEW_COL_RESIZABLE );
	bSizer->Add( m_dataViewListCtrl2, 1, wxALL|wxEXPAND, 5 );


	this->SetSizer( bSizer );
	this->Layout();
	m_menubar = new wxMenuBar( 0 );
	m_menu14 = new wxMenu();
	wxMenuItem* m_menuItemLoadModel;
	m_menuItemLoadModel = new wxMenuItem( m_menu14, wxID_ANY, wxString( _("Load Model") ) + wxT('\t') + wxT("Ctrl+L"), wxEmptyString, wxITEM_NORMAL );
	m_menu14->Append( m_menuItemLoadModel );

	wxMenuItem* m_menuItemSave;
	m_menuItemSave = new wxMenuItem( m_menu14, wxID_ANY, wxString( _("&Save Model") ) + wxT('\t') + wxT("Ctrl+S"), wxEmptyString, wxITEM_NORMAL );
	m_menu14->Append( m_menuItemSave );

	m_menu14->AppendSeparator();

	wxMenuItem* m_menuItemClose;
	m_menuItemClose = new wxMenuItem( m_menu14, wxID_ANY, wxString( _("&Close") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu14->Append( m_menuItemClose );

	m_menubar->Append( m_menu14, _("&File") );

	this->SetMenuBar( m_menubar );


	this->Centre( wxBOTH );

	// Connect Events
	m_menu14->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameEditorFootModel::OnLoad ), this, m_menuItemLoadModel->GetId());
	m_menu14->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameEditorFootModel::OnSave ), this, m_menuItemSave->GetId());
	m_menu14->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameEditorFootModel::OnClose ), this, m_menuItemClose->GetId());
}

GUIFrameEditorFootModel::~GUIFrameEditorFootModel()
{
	// Disconnect Events

}

GUIDialogQuickInitFoot::GUIDialogQuickInitFoot( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizerMain;
	bSizerMain = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizerSize;
	bSizerSize = new wxBoxSizer( wxHORIZONTAL );

	m_staticTextShoeSize = new wxStaticText( this, wxID_ANY, _("Shoesize:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextShoeSize->Wrap( -1 );
	bSizerSize->Add( m_staticTextShoeSize, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrlShoeSize = new ExtendedTextCtrl( this, wxID_ANY, _("39"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT );
	bSizerSize->Add( m_textCtrlShoeSize, 0, wxALL|wxEXPAND, 5 );

	wxString m_choiceUnitChoices[] = { _("EU"), _("US"), _("CN"), _("UK"), _("JP"), _("AU"), _("mm"), _("cm"), _("in"), _("ft") };
	int m_choiceUnitNChoices = sizeof( m_choiceUnitChoices ) / sizeof( wxString );
	m_choiceUnit = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceUnitNChoices, m_choiceUnitChoices, 0 );
	m_choiceUnit->SetSelection( 0 );
	bSizerSize->Add( m_choiceUnit, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	bSizerMain->Add( bSizerSize, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizerWidth;
	bSizerWidth = new wxBoxSizer( wxHORIZONTAL );

	m_radioBtnSmall = new wxRadioButton( this, wxID_ANY, _("small"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	bSizerWidth->Add( m_radioBtnSmall, 0, wxALL, 5 );

	m_radioBtnMedium = new wxRadioButton( this, wxID_ANY, _("medium"), wxDefaultPosition, wxDefaultSize, 0 );
	m_radioBtnMedium->SetValue( true );
	bSizerWidth->Add( m_radioBtnMedium, 0, wxALL, 5 );

	m_radioBtnWide = new wxRadioButton( this, wxID_ANY, _("wide"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerWidth->Add( m_radioBtnWide, 0, wxALL, 5 );


	bSizerMain->Add( bSizerWidth, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

	wxBoxSizer* bSizerButtons;
	bSizerButtons = new wxBoxSizer( wxHORIZONTAL );

	m_buttonInit = new wxButton( this, wxID_OK, _("Init"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerButtons->Add( m_buttonInit, 1, wxALL, 5 );

	m_buttonAbort = new wxButton( this, wxID_CANCEL, _("Abort"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerButtons->Add( m_buttonAbort, 1, wxALL, 5 );


	bSizerMain->Add( bSizerButtons, 0, wxEXPAND, 5 );


	this->SetSizer( bSizerMain );
	this->Layout();
	bSizerMain->Fit( this );

	// Connect Events
	m_textCtrlShoeSize->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIDialogQuickInitFoot::OnTextEnter ), NULL, this );
	m_buttonInit->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIDialogQuickInitFoot::OnClose ), NULL, this );
}

GUIDialogQuickInitFoot::~GUIDialogQuickInitFoot()
{
	// Disconnect Events
	m_textCtrlShoeSize->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIDialogQuickInitFoot::OnTextEnter ), NULL, this );
	m_buttonInit->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIDialogQuickInitFoot::OnClose ), NULL, this );

}

GUIFrameDebugParser::GUIFrameDebugParser( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer;
	bSizer = new wxBoxSizer( wxVERTICAL );

	m_staticText10 = new wxStaticText( this, wxID_ANY, _("Input (Number / Calculation / Formula / Unit):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	bSizer->Add( m_staticText10, 0, wxALL, 5 );

	m_textCtrlExpression = new ExtendedTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer->Add( m_textCtrlExpression, 0, wxALL|wxEXPAND, 5 );

	m_staticText11 = new wxStaticText( this, wxID_ANY, _("Numeric Value:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	bSizer->Add( m_staticText11, 0, wxALL, 5 );

	m_textCtrlNumber = new ExtendedTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	bSizer->Add( m_textCtrlNumber, 0, wxALL|wxEXPAND, 5 );

	m_staticText12 = new wxStaticText( this, wxID_ANY, _("Error:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	bSizer->Add( m_staticText12, 0, wxALL, 5 );

	m_textCtrlError = new ExtendedTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	bSizer->Add( m_textCtrlError, 0, wxALL|wxEXPAND, 5 );

	m_staticText14 = new wxStaticText( this, wxID_ANY, _("Unit Text:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	bSizer->Add( m_staticText14, 0, wxALL, 5 );

	m_textCtrlUnit = new ExtendedTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	bSizer->Add( m_textCtrlUnit, 0, wxALL|wxEXPAND, 5 );

	m_staticText13 = new wxStaticText( this, wxID_ANY, _("SI Base Units:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	bSizer->Add( m_staticText13, 0, wxALL, 5 );

	m_textCtrlSIBase = new ExtendedTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	m_textCtrlSIBase->Enable( false );

	bSizer->Add( m_textCtrlSIBase, 0, wxALL|wxEXPAND, 5 );


	this->SetSizer( bSizer );
	this->Layout();
	bSizer->Fit( this );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GUIFrameDebugParser::OnCloseX ) );
	m_textCtrlExpression->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GUIFrameDebugParser::OnText ), NULL, this );
}

GUIFrameDebugParser::~GUIFrameDebugParser()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GUIFrameDebugParser::OnCloseX ) );
	m_textCtrlExpression->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GUIFrameDebugParser::OnText ), NULL, this );

}

GUIDialogFormulaEditor::GUIDialogFormulaEditor( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizerMain;
	bSizerMain = new wxBoxSizer( wxVERTICAL );

	m_dataViewListCtrl = new wxDataViewListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_HORIZ_RULES|wxDV_SINGLE|wxDV_VERT_RULES );
	m_dataViewListCtrl->SetToolTip( _("Available variables for formula.\nDouble-click to copy to cursor position.") );

	m_dataViewListColumnVariable = m_dataViewListCtrl->AppendTextColumn( _("Variable"), wxDATAVIEW_CELL_INERT, -1, static_cast<wxAlignment>(wxALIGN_LEFT), wxDATAVIEW_COL_RESIZABLE );
	m_dataViewListColumnFormula = m_dataViewListCtrl->AppendTextColumn( _("Formula"), wxDATAVIEW_CELL_INERT, -1, static_cast<wxAlignment>(wxALIGN_LEFT), wxDATAVIEW_COL_RESIZABLE );
	m_dataViewListColumnValue = m_dataViewListCtrl->AppendTextColumn( _("Value"), wxDATAVIEW_CELL_INERT, -1, static_cast<wxAlignment>(wxALIGN_LEFT), wxDATAVIEW_COL_RESIZABLE );
	bSizerMain->Add( m_dataViewListCtrl, 1, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizerVariableName;
	bSizerVariableName = new wxBoxSizer( wxHORIZONTAL );

	m_staticTextVariableName = new wxStaticText( this, wxID_ANY, _("<>"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextVariableName->Wrap( -1 );
	bSizerVariableName->Add( m_staticTextVariableName, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	bSizerMain->Add( bSizerVariableName, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizerFormulaAndUnit;
	bSizerFormulaAndUnit = new wxBoxSizer( wxHORIZONTAL );

	m_textCtrlFormula = new ExtendedTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrlFormula->SetToolTip( _("Enter formula or value. \nAllowed operations are: +, -, *, /, ^, cos, sin, tan, acos, asin, atan, exp, log, sqrt, cbrt, ceil, floor, round, abs") );

	bSizerFormulaAndUnit->Add( m_textCtrlFormula, 1, wxALL, 5 );

	m_textCtrlUnit = new ExtendedTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	m_textCtrlUnit->SetToolTip( _("Unit") );

	bSizerFormulaAndUnit->Add( m_textCtrlUnit, 0, wxALL, 5 );


	bSizerMain->Add( bSizerFormulaAndUnit, 0, wxEXPAND, 5 );

	m_textCtrlResult = new ExtendedTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	m_textCtrlResult->SetToolTip( _("Result") );

	bSizerMain->Add( m_textCtrlResult, 0, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizerButtons;
	bSizerButtons = new wxBoxSizer( wxHORIZONTAL );

	m_buttonClear = new wxButton( this, wxID_ANY, _("Clear formula"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerButtons->Add( m_buttonClear, 0, wxALL, 5 );

	m_buttonCopyResultFormula = new wxButton( this, wxID_ANY, _("Copy result to formula"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerButtons->Add( m_buttonCopyResultFormula, 0, wxALL, 5 );


	bSizerButtons->Add( 0, 0, 1, wxEXPAND, 5 );

	m_buttonCancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerButtons->Add( m_buttonCancel, 0, wxALL, 5 );

	m_buttonOK = new wxButton( this, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerButtons->Add( m_buttonOK, 0, wxALL, 5 );


	bSizerMain->Add( bSizerButtons, 0, wxEXPAND, 5 );


	this->SetSizer( bSizerMain );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_dataViewListCtrl->Connect( wxEVT_COMMAND_DATAVIEW_ITEM_ACTIVATED, wxDataViewEventHandler( GUIDialogFormulaEditor::OnItemActivated ), NULL, this );
	m_buttonClear->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIDialogFormulaEditor::OnClearFormula ), NULL, this );
	m_buttonCopyResultFormula->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIDialogFormulaEditor::OnCopyResultToFormula ), NULL, this );
}

GUIDialogFormulaEditor::~GUIDialogFormulaEditor()
{
	// Disconnect Events
	m_dataViewListCtrl->Disconnect( wxEVT_COMMAND_DATAVIEW_ITEM_ACTIVATED, wxDataViewEventHandler( GUIDialogFormulaEditor::OnItemActivated ), NULL, this );
	m_buttonClear->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIDialogFormulaEditor::OnClearFormula ), NULL, this );
	m_buttonCopyResultFormula->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIDialogFormulaEditor::OnCopyResultToFormula ), NULL, this );

}

GUIFrameSetupUnits::GUIFrameSetupUnits( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BACKGROUND ) );

	wxBoxSizer* bSizer25;
	bSizer25 = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* sbSizer6;
	sbSizer6 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Units ") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer6;
	fgSizer6 = new wxFlexGridSizer( 3, 2, 0, 0 );
	fgSizer6->SetFlexibleDirection( wxBOTH );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText57 = new wxStaticText( sbSizer6->GetStaticBox(), wxID_ANY, _("Length"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText57->Wrap( -1 );
	fgSizer6->Add( m_staticText57, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxArrayString m_choiceUnitLengthChoices;
	m_choiceUnitLength = new wxChoice( sbSizer6->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceUnitLengthChoices, 0 );
	m_choiceUnitLength->SetSelection( 0 );
	fgSizer6->Add( m_choiceUnitLength, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText110 = new wxStaticText( sbSizer6->GetStaticBox(), wxID_ANY, _("Small Distance"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText110->Wrap( -1 );
	fgSizer6->Add( m_staticText110, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxArrayString m_choiceUnitSmallDistanceChoices;
	m_choiceUnitSmallDistance = new wxChoice( sbSizer6->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceUnitSmallDistanceChoices, 0 );
	m_choiceUnitSmallDistance->SetSelection( 0 );
	fgSizer6->Add( m_choiceUnitSmallDistance, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText112 = new wxStaticText( sbSizer6->GetStaticBox(), wxID_ANY, _("Angle"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText112->Wrap( -1 );
	fgSizer6->Add( m_staticText112, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxArrayString m_choiceUnitAngleChoices;
	m_choiceUnitAngle = new wxChoice( sbSizer6->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceUnitAngleChoices, 0 );
	m_choiceUnitAngle->SetSelection( 0 );
	fgSizer6->Add( m_choiceUnitAngle, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );


	sbSizer6->Add( fgSizer6, 0, 0, 5 );


	bSizer25->Add( sbSizer6, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

	m_buttonClose = new wxButton( this, wxID_ANY, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer25->Add( m_buttonClose, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_staticText64 = new wxStaticText( this, wxID_ANY, _("Unit conversion is only used for displaying.\nInternally everything is stored in SI base units."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText64->Wrap( -1 );
	bSizer25->Add( m_staticText64, 0, wxALL|wxEXPAND, 5 );


	this->SetSizer( bSizer25 );
	this->Layout();
	bSizer25->Fit( this );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GUIFrameSetupUnits::OnCloseX ) );
	m_choiceUnitLength->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GUIFrameSetupUnits::OnChangeUnit ), NULL, this );
	m_choiceUnitSmallDistance->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GUIFrameSetupUnits::OnChangeUnit ), NULL, this );
	m_choiceUnitAngle->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GUIFrameSetupUnits::OnChangeUnit ), NULL, this );
	m_buttonClose->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameSetupUnits::OnClose ), NULL, this );
}

GUIFrameSetupUnits::~GUIFrameSetupUnits()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GUIFrameSetupUnits::OnCloseX ) );
	m_choiceUnitLength->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GUIFrameSetupUnits::OnChangeUnit ), NULL, this );
	m_choiceUnitSmallDistance->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GUIFrameSetupUnits::OnChangeUnit ), NULL, this );
	m_choiceUnitAngle->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GUIFrameSetupUnits::OnChangeUnit ), NULL, this );
	m_buttonClose->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameSetupUnits::OnClose ), NULL, this );

}

GUIFrameSetupBackgroundImages::GUIFrameSetupBackgroundImages( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizerMain;
	bSizerMain = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizerLeftColumn;
	bSizerLeftColumn = new wxBoxSizer( wxVERTICAL );

	m_listCtrl = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_ICON );
	bSizerLeftColumn->Add( m_listCtrl, 1, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizerButtons;
	bSizerButtons = new wxBoxSizer( wxHORIZONTAL );

	m_buttonAdd = new wxButton( this, wxID_ANY, _("Add/Update"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerButtons->Add( m_buttonAdd, 1, wxALL, 5 );

	m_buttonRemove = new wxButton( this, wxID_ANY, _("Remove"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerButtons->Add( m_buttonRemove, 1, wxALL, 5 );


	bSizerLeftColumn->Add( bSizerButtons, 0, wxEXPAND, 5 );


	bSizerMain->Add( bSizerLeftColumn, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizerRightColumn;
	bSizerRightColumn = new wxBoxSizer( wxVERTICAL );

	m_staticTextFilename = new wxStaticText( this, wxID_ANY, _("Filename:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextFilename->Wrap( -1 );
	bSizerRightColumn->Add( m_staticTextFilename, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	m_filePicker = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, _("Select a file"), _("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	bSizerRightColumn->Add( m_filePicker, 0, wxALL|wxEXPAND, 5 );

	m_staticTextDirection = new wxStaticText( this, wxID_ANY, _("Direction:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDirection->Wrap( -1 );
	bSizerRightColumn->Add( m_staticTextDirection, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	wxGridSizer* gSizerDirection;
	gSizerDirection = new wxGridSizer( 0, 2, 0, 0 );

	m_checkBoxFront = new wxCheckBox( this, wxID_ANY, _("Front"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizerDirection->Add( m_checkBoxFront, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	m_checkBoxBack = new wxCheckBox( this, wxID_ANY, _("Back"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizerDirection->Add( m_checkBoxBack, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	m_checkBoxLeft = new wxCheckBox( this, wxID_ANY, _("Left"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizerDirection->Add( m_checkBoxLeft, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	m_checkBoxRight = new wxCheckBox( this, wxID_ANY, _("Right"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizerDirection->Add( m_checkBoxRight, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	m_checkBoxTop = new wxCheckBox( this, wxID_ANY, _("Top"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizerDirection->Add( m_checkBoxTop, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	m_checkBoxBottom = new wxCheckBox( this, wxID_ANY, _("Bottom"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizerDirection->Add( m_checkBoxBottom, 0, wxTOP|wxRIGHT|wxLEFT, 5 );


	bSizerRightColumn->Add( gSizerDirection, 0, wxEXPAND|wxBOTTOM, 5 );

	m_staticText46 = new wxStaticText( this, wxID_ANY, _("Scale:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText46->Wrap( -1 );
	bSizerRightColumn->Add( m_staticText46, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	wxBoxSizer* bSizerScale;
	bSizerScale = new wxBoxSizer( wxHORIZONTAL );

	m_textCtrlScale = new ExtendedTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizerScale->Add( m_textCtrlScale, 1, wxTOP|wxBOTTOM|wxLEFT, 5 );

	m_spinBtnScale = new wxSpinButton( this, ID_SPINSCALE, wxDefaultPosition, wxDefaultSize, 0 );
	bSizerScale->Add( m_spinBtnScale, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );


	bSizerRightColumn->Add( bSizerScale, 0, wxEXPAND, 5 );

	m_staticTextOffsHor = new wxStaticText( this, wxID_ANY, _("Horizontal offset:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextOffsHor->Wrap( -1 );
	bSizerRightColumn->Add( m_staticTextOffsHor, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	wxBoxSizer* bSizerHorizontalOffset;
	bSizerHorizontalOffset = new wxBoxSizer( wxHORIZONTAL );

	m_textCtrlOffsHor = new ExtendedTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizerHorizontalOffset->Add( m_textCtrlOffsHor, 1, wxTOP|wxBOTTOM|wxLEFT, 5 );

	m_spinBtnOffsHor = new wxSpinButton( this, ID_SPINOFFSHOR, wxDefaultPosition, wxDefaultSize, 0 );
	bSizerHorizontalOffset->Add( m_spinBtnOffsHor, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );


	bSizerRightColumn->Add( bSizerHorizontalOffset, 0, wxEXPAND, 5 );

	m_staticTextOffsVert = new wxStaticText( this, wxID_ANY, _("Vertical Offset:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextOffsVert->Wrap( -1 );
	bSizerRightColumn->Add( m_staticTextOffsVert, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	wxBoxSizer* bSizerVerticalOffset;
	bSizerVerticalOffset = new wxBoxSizer( wxHORIZONTAL );

	m_textCtrlOffsVert = new ExtendedTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizerVerticalOffset->Add( m_textCtrlOffsVert, 1, wxTOP|wxBOTTOM|wxLEFT, 5 );

	m_spinBtnOffsVert = new wxSpinButton( this, ID_SPINOFFSVERT, wxDefaultPosition, wxDefaultSize, 0 );
	bSizerVerticalOffset->Add( m_spinBtnOffsVert, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );


	bSizerRightColumn->Add( bSizerVerticalOffset, 0, wxEXPAND, 5 );

	m_staticTextRotation = new wxStaticText( this, wxID_ANY, _("Rotation:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextRotation->Wrap( -1 );
	bSizerRightColumn->Add( m_staticTextRotation, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	m_sliderRotation = new wxSlider( this, wxID_ANY, 0, -180, 180, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_HORIZONTAL|wxSL_LABELS );
	bSizerRightColumn->Add( m_sliderRotation, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );


	bSizerRightColumn->Add( 0, 0, 1, wxEXPAND, 5 );

	m_buttonClose = new wxButton( this, ID_CLOSE, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerRightColumn->Add( m_buttonClose, 0, wxALL|wxEXPAND, 5 );


	bSizerMain->Add( bSizerRightColumn, 1, wxEXPAND, 5 );


	this->SetSizer( bSizerMain );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_buttonAdd->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameSetupBackgroundImages::OnAddUpdate ), NULL, this );
	m_buttonRemove->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameSetupBackgroundImages::OnRemove ), NULL, this );
	m_textCtrlScale->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameSetupBackgroundImages::OnTextEnter ), NULL, this );
	m_spinBtnScale->Connect( wxEVT_SCROLL_THUMBTRACK, wxSpinEventHandler( GUIFrameSetupBackgroundImages::OnSpin ), NULL, this );
	m_textCtrlOffsHor->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameSetupBackgroundImages::OnTextEnter ), NULL, this );
	m_spinBtnOffsHor->Connect( wxEVT_SCROLL_THUMBTRACK, wxSpinEventHandler( GUIFrameSetupBackgroundImages::OnSpin ), NULL, this );
	m_textCtrlOffsVert->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameSetupBackgroundImages::OnTextEnter ), NULL, this );
	m_spinBtnOffsVert->Connect( wxEVT_SCROLL_THUMBTRACK, wxSpinEventHandler( GUIFrameSetupBackgroundImages::OnSpin ), NULL, this );
	m_sliderRotation->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( GUIFrameSetupBackgroundImages::OnRightDown ), NULL, this );
	m_sliderRotation->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( GUIFrameSetupBackgroundImages::OnScroll ), NULL, this );
	m_sliderRotation->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( GUIFrameSetupBackgroundImages::OnScroll ), NULL, this );
	m_sliderRotation->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( GUIFrameSetupBackgroundImages::OnScroll ), NULL, this );
	m_sliderRotation->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( GUIFrameSetupBackgroundImages::OnScroll ), NULL, this );
	m_sliderRotation->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( GUIFrameSetupBackgroundImages::OnScroll ), NULL, this );
	m_sliderRotation->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( GUIFrameSetupBackgroundImages::OnScroll ), NULL, this );
	m_sliderRotation->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( GUIFrameSetupBackgroundImages::OnScroll ), NULL, this );
	m_sliderRotation->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( GUIFrameSetupBackgroundImages::OnScroll ), NULL, this );
	m_sliderRotation->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( GUIFrameSetupBackgroundImages::OnScroll ), NULL, this );
}

GUIFrameSetupBackgroundImages::~GUIFrameSetupBackgroundImages()
{
	// Disconnect Events
	m_buttonAdd->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameSetupBackgroundImages::OnAddUpdate ), NULL, this );
	m_buttonRemove->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameSetupBackgroundImages::OnRemove ), NULL, this );
	m_textCtrlScale->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameSetupBackgroundImages::OnTextEnter ), NULL, this );
	m_spinBtnScale->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxSpinEventHandler( GUIFrameSetupBackgroundImages::OnSpin ), NULL, this );
	m_textCtrlOffsHor->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameSetupBackgroundImages::OnTextEnter ), NULL, this );
	m_spinBtnOffsHor->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxSpinEventHandler( GUIFrameSetupBackgroundImages::OnSpin ), NULL, this );
	m_textCtrlOffsVert->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameSetupBackgroundImages::OnTextEnter ), NULL, this );
	m_spinBtnOffsVert->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxSpinEventHandler( GUIFrameSetupBackgroundImages::OnSpin ), NULL, this );
	m_sliderRotation->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( GUIFrameSetupBackgroundImages::OnRightDown ), NULL, this );
	m_sliderRotation->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( GUIFrameSetupBackgroundImages::OnScroll ), NULL, this );
	m_sliderRotation->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( GUIFrameSetupBackgroundImages::OnScroll ), NULL, this );
	m_sliderRotation->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( GUIFrameSetupBackgroundImages::OnScroll ), NULL, this );
	m_sliderRotation->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( GUIFrameSetupBackgroundImages::OnScroll ), NULL, this );
	m_sliderRotation->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( GUIFrameSetupBackgroundImages::OnScroll ), NULL, this );
	m_sliderRotation->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( GUIFrameSetupBackgroundImages::OnScroll ), NULL, this );
	m_sliderRotation->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( GUIFrameSetupBackgroundImages::OnScroll ), NULL, this );
	m_sliderRotation->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( GUIFrameSetupBackgroundImages::OnScroll ), NULL, this );
	m_sliderRotation->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( GUIFrameSetupBackgroundImages::OnScroll ), NULL, this );

}

GUISetupPaths::GUISetupPaths( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer;
	bSizer = new wxBoxSizer( wxVERTICAL );

	m_staticTextMeasurementsDirectory = new wxStaticText( this, wxID_ANY, _("Directory for measurements:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextMeasurementsDirectory->Wrap( -1 );
	bSizer->Add( m_staticTextMeasurementsDirectory, 0, wxALL, 5 );

	m_dirPickerMeasurementsDirectory = new wxDirPickerCtrl( this, wxID_ANY, wxEmptyString, _("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE|wxDIRP_DIR_MUST_EXIST|wxDIRP_USE_TEXTCTRL );
	bSizer->Add( m_dirPickerMeasurementsDirectory, 0, wxALL|wxEXPAND, 5 );

	m_staticTextShoeDesignDirectory = new wxStaticText( this, wxID_ANY, _("Directory for shoe designs:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextShoeDesignDirectory->Wrap( -1 );
	bSizer->Add( m_staticTextShoeDesignDirectory, 0, wxALL, 5 );

	m_dirPickerShoeDesignDirectory = new wxDirPickerCtrl( this, wxID_ANY, wxEmptyString, _("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE|wxDIRP_DIR_MUST_EXIST|wxDIRP_USE_TEXTCTRL );
	bSizer->Add( m_dirPickerShoeDesignDirectory, 0, wxALL|wxEXPAND, 5 );

	m_staticTextLastDirectory = new wxStaticText( this, wxID_ANY, _("Directory for lasts:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextLastDirectory->Wrap( -1 );
	bSizer->Add( m_staticTextLastDirectory, 0, wxALL, 5 );

	m_dirPickerLastDirectory = new wxDirPickerCtrl( this, wxID_ANY, wxEmptyString, _("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE|wxDIRP_DIR_MUST_EXIST|wxDIRP_USE_TEXTCTRL );
	bSizer->Add( m_dirPickerLastDirectory, 0, wxALL|wxEXPAND, 5 );

	m_staticTextOutputDirectory = new wxStaticText( this, wxID_ANY, _("Output directory:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextOutputDirectory->Wrap( -1 );
	bSizer->Add( m_staticTextOutputDirectory, 0, wxALL, 5 );

	m_dirPickerOutputDirectory = new wxDirPickerCtrl( this, wxID_ANY, wxEmptyString, _("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE|wxDIRP_DIR_MUST_EXIST|wxDIRP_USE_TEXTCTRL );
	bSizer->Add( m_dirPickerOutputDirectory, 0, wxALL|wxEXPAND, 5 );

	m_buttonClose = new wxButton( this, wxID_ANY, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer->Add( m_buttonClose, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );


	this->SetSizer( bSizer );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_buttonClose->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUISetupPaths::OnClose ), NULL, this );
}

GUISetupPaths::~GUISetupPaths()
{
	// Disconnect Events
	m_buttonClose->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUISetupPaths::OnClose ), NULL, this );

}

GUIDialogMidiSetup::GUIDialogMidiSetup( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer;
	bSizer = new wxBoxSizer( wxVERTICAL );

	m_staticText = new wxStaticText( this, wxID_ANY, _("Midi-Controller:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText->Wrap( -1 );
	bSizer->Add( m_staticText, 0, wxALL, 5 );

	wxArrayString m_choiceChoices;
	m_choice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceChoices, 0 );
	m_choice->SetSelection( 0 );
	bSizer->Add( m_choice, 0, wxALL|wxEXPAND, 5 );

	m_buttonConnectDisconnect = new wxButton( this, wxID_ANY, _("Connect"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer->Add( m_buttonConnectDisconnect, 0, wxALL|wxEXPAND, 5 );

	m_buttonClose = new wxButton( this, wxID_CLOSE, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer->Add( m_buttonClose, 0, wxALL|wxEXPAND, 5 );


	this->SetSizer( bSizer );
	this->Layout();

	// Connect Events
	m_buttonConnectDisconnect->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIDialogMidiSetup::OnConnectDisconnect ), NULL, this );
	m_buttonClose->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIDialogMidiSetup::OnClose ), NULL, this );
}

GUIDialogMidiSetup::~GUIDialogMidiSetup()
{
	// Disconnect Events
	m_buttonConnectDisconnect->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIDialogMidiSetup::OnConnectDisconnect ), NULL, this );
	m_buttonClose->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIDialogMidiSetup::OnClose ), NULL, this );

}

GUIDialogAnisotropy::GUIDialogAnisotropy( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizerAnisotropy;
	bSizerAnisotropy = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizerPoints;
	bSizerPoints = new wxBoxSizer( wxHORIZONTAL );

	m_buttonAdd = new wxButton( this, ID_POINTADD, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerPoints->Add( m_buttonAdd, 0, wxALL, 5 );

	m_buttonRemove = new wxButton( this, ID_POINTREMOVE, _("Remove"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerPoints->Add( m_buttonRemove, 0, wxALL, 5 );


	bSizerAnisotropy->Add( bSizerPoints, 0, wxEXPAND, 5 );

	m_panelAnisotropy = new PanelAnisotropy(this);
	m_panelAnisotropy->Hide();

	bSizerAnisotropy->Add( m_panelAnisotropy, 0, wxALL, 5 );


	this->SetSizer( bSizerAnisotropy );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_buttonAdd->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIDialogAnisotropy::OnAddPoint ), NULL, this );
	m_buttonRemove->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIDialogAnisotropy::OnRemovePoint ), NULL, this );
}

GUIDialogAnisotropy::~GUIDialogAnisotropy()
{
	// Disconnect Events
	m_buttonAdd->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIDialogAnisotropy::OnAddPoint ), NULL, this );
	m_buttonRemove->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIDialogAnisotropy::OnRemovePoint ), NULL, this );

}
