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
	m_menuItemQuickSetup = new wxMenuItem( m_menuFoot, ID_MEASUREMENTS_SETUP, wxString( _("Quick &Setup") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuFoot->Append( m_menuItemQuickSetup );

	m_menuFoot->AppendSeparator();

	m_menuFootParameters = new wxMenu();
	wxMenuItem* m_menuFootParametersItem = new wxMenuItem( m_menuFoot, wxID_ANY, _("Foot &parameters"), wxEmptyString, wxITEM_NORMAL, m_menuFootParameters );
	wxMenuItem* m_menuItemFromMeasurements;
	m_menuItemFromMeasurements = new wxMenuItem( m_menuFootParameters, ID_MEASUREMENTS, wxString( _("from &measurements") ) , wxEmptyString, wxITEM_RADIO );
	m_menuFootParameters->Append( m_menuItemFromMeasurements );
	m_menuItemFromMeasurements->Check( true );

	wxMenuItem* m_menuItemFromScan;
	m_menuItemFromScan = new wxMenuItem( m_menuFootParameters, ID_SCAN, wxString( _("from &scan") ) , wxEmptyString, wxITEM_RADIO );
	m_menuFootParameters->Append( m_menuItemFromScan );

	m_menuFoot->Append( m_menuFootParametersItem );

	m_menuMeasurementsCopy = new wxMenu();
	wxMenuItem* m_menuMeasurementsCopyItem = new wxMenuItem( m_menuFoot, wxID_ANY, _("&Copy measurements"), wxEmptyString, wxITEM_NORMAL, m_menuMeasurementsCopy );
	wxMenuItem* m_menuItemMeasurementsCopyActiveToInactive;
	m_menuItemMeasurementsCopyActiveToInactive = new wxMenuItem( m_menuMeasurementsCopy, ID_ACTIVETOINACTIVE, wxString( _("from active side to inactive side") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuMeasurementsCopy->Append( m_menuItemMeasurementsCopyActiveToInactive );

	wxMenuItem* m_menuItemMeasurementsCopyRightToLeft;
	m_menuItemMeasurementsCopyRightToLeft = new wxMenuItem( m_menuMeasurementsCopy, ID_RIGHTTOLEFT, wxString( _("from right to left") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuMeasurementsCopy->Append( m_menuItemMeasurementsCopyRightToLeft );

	wxMenuItem* m_menuItemMeasurementsCopyLeftToRight;
	m_menuItemMeasurementsCopyLeftToRight = new wxMenuItem( m_menuMeasurementsCopy, ID_LEFTTORIGHT, wxString( _("from left to right") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuMeasurementsCopy->Append( m_menuItemMeasurementsCopyLeftToRight );

	m_menuFoot->Append( m_menuMeasurementsCopyItem );

	m_menubar->Append( m_menuFoot, _("F&oot/Leg") );

	m_menuLast = new wxMenu();
	m_menuLastModel = new wxMenu();
	wxMenuItem* m_menuLastModelItem = new wxMenuItem( m_menuLast, wxID_ANY, _("Foot &model"), wxEmptyString, wxITEM_NORMAL, m_menuLastModel );
	wxMenuItem* m_menuItemConstructedLast;
	m_menuItemConstructedLast = new wxMenuItem( m_menuLastModel, ID_CONSTRUCTION, wxString( _("Constructed last") ) , wxEmptyString, wxITEM_RADIO );
	m_menuLastModel->Append( m_menuItemConstructedLast );
	m_menuItemConstructedLast->Check( true );

	wxMenuItem* m_menuItemBoneBasedModel;
	m_menuItemBoneBasedModel = new wxMenuItem( m_menuLastModel, ID_BONES, wxString( _("Bone-based last model") ) , wxEmptyString, wxITEM_RADIO );
	m_menuLastModel->Append( m_menuItemBoneBasedModel );
	m_menuItemBoneBasedModel->Check( true );

	wxMenuItem* m_menuItemUseLastBasedModel;
	m_menuItemUseLastBasedModel = new wxMenuItem( m_menuLastModel, ID_LAST, wxString( _("Load last from file") ) , wxEmptyString, wxITEM_RADIO );
	m_menuLastModel->Append( m_menuItemUseLastBasedModel );

	m_menuLast->Append( m_menuLastModelItem );

	wxMenuItem* m_menuItemLastLoadSTL;
	m_menuItemLastLoadSTL = new wxMenuItem( m_menuLast, ID_LAST, wxString( _("Load Last from STL") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuLast->Append( m_menuItemLastLoadSTL );

	m_menuLast->AppendSeparator();

	m_menuBoneModel = new wxMenu();
	wxMenuItem* m_menuBoneModelItem = new wxMenuItem( m_menuLast, wxID_ANY, _("&Foot bone model"), wxEmptyString, wxITEM_NORMAL, m_menuBoneModel );
	wxMenuItem* m_menuItemEditBoneModel;
	m_menuItemEditBoneModel = new wxMenuItem( m_menuBoneModel, ID_BONES_EDIT, wxString( _("&Edit bone model") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuBoneModel->Append( m_menuItemEditBoneModel );

	wxMenuItem* m_menuItemLoadBoneModel;
	m_menuItemLoadBoneModel = new wxMenuItem( m_menuBoneModel, ID_BONES, wxString( _("&Load bone model") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuBoneModel->Append( m_menuItemLoadBoneModel );

	m_menuLast->Append( m_menuBoneModelItem );

	m_menubar->Append( m_menuLast, _("&Last") );

	m_menuShoe = new wxMenu();
	m_menuConstruction = new wxMenu();
	wxMenuItem* m_menuConstructionItem = new wxMenuItem( m_menuShoe, wxID_ANY, _("&Construction method"), wxEmptyString, wxITEM_NORMAL, m_menuConstruction );
	wxMenuItem* m_menuItemTestGenerator;
	m_menuItemTestGenerator = new wxMenuItem( m_menuConstruction, ID_CONSTRUCTION_EXPERIMENTAL, wxString( _("&Experimental generator") ) , _("Generator for alogithm development"), wxITEM_RADIO );
	m_menuConstruction->Append( m_menuItemTestGenerator );

	wxMenuItem* m_menuItemConstructionClassic;
	m_menuItemConstructionClassic = new wxMenuItem( m_menuConstruction, ID_CONSTRUCTION_WELDED, wxString( _("Classic, &welded construction") ) , wxEmptyString, wxITEM_RADIO );
	m_menuConstruction->Append( m_menuItemConstructionClassic );

	wxMenuItem* m_menuItemConstructionCemented;
	m_menuItemConstructionCemented = new wxMenuItem( m_menuConstruction, ID_CONSTRUCTION_CEMENTED, wxString( _("&Cemented construction") ) , wxEmptyString, wxITEM_RADIO );
	m_menuConstruction->Append( m_menuItemConstructionCemented );

	wxMenuItem* m_menuItemConstructionMolded;
	m_menuItemConstructionMolded = new wxMenuItem( m_menuConstruction, ID_CONSTRUCTION_MOLDED, wxString( _("Injection-&molded sole") ) , wxEmptyString, wxITEM_RADIO );
	m_menuConstruction->Append( m_menuItemConstructionMolded );

	wxMenuItem* m_menuItemConstructionDutch;
	m_menuItemConstructionDutch = new wxMenuItem( m_menuConstruction, ID_CONSTRUCTION_DUTCH, wxString( _("&Dutch clog") ) , wxEmptyString, wxITEM_RADIO );
	m_menuConstruction->Append( m_menuItemConstructionDutch );

	wxMenuItem* m_menuItemConstructionGeta;
	m_menuItemConstructionGeta = new wxMenuItem( m_menuConstruction, ID_CONSTRUCTION_GETA, wxString( _("&Japanese Geta") ) , wxEmptyString, wxITEM_RADIO );
	m_menuConstruction->Append( m_menuItemConstructionGeta );

	m_menuShoe->Append( m_menuConstructionItem );

	m_menubar->Append( m_menuShoe, _("&Shoe/Heel") );

	m_menuPattern = new wxMenu();
	wxMenuItem* m_menuItemLoadPattern;
	m_menuItemLoadPattern = new wxMenuItem( m_menuPattern, ID_PATTERN, wxString( _("&Load Pattern") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuPattern->Append( m_menuItemLoadPattern );
	m_menuItemLoadPattern->Enable( false );

	wxMenuItem* m_menuItemSavePattern;
	m_menuItemSavePattern = new wxMenuItem( m_menuPattern, ID_PATTERN_SAVE, wxString( _("&Save Pattern") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuPattern->Append( m_menuItemSavePattern );
	m_menuItemSavePattern->Enable( false );

	wxMenuItem* m_menuItemShowFlattening;
	m_menuItemShowFlattening = new wxMenuItem( m_menuPattern, ID_FLATTENING, wxString( _("Show Flattening") ) , wxEmptyString, wxITEM_CHECK );
	m_menuPattern->Append( m_menuItemShowFlattening );
	m_menuItemShowFlattening->Enable( false );

	m_menubar->Append( m_menuPattern, _("&Pattern") );

	m_menuFabric = new wxMenu();
	wxMenuItem* m_menuItemLoadFabricsLibrary;
	m_menuItemLoadFabricsLibrary = new wxMenuItem( m_menuFabric, ID_FABRICS, wxString( _("&Load Fabrics Library") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuFabric->Append( m_menuItemLoadFabricsLibrary );
	m_menuItemLoadFabricsLibrary->Enable( false );

	m_menubar->Append( m_menuFabric, _("F&abric") );

	m_menuGeometry = new wxMenu();
	wxMenuItem* m_menuItemSaveLast;
	m_menuItemSaveLast = new wxMenuItem( m_menuGeometry, ID_LAST, wxString( _("Save &Last") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuGeometry->Append( m_menuItemSaveLast );

	wxMenuItem* m_menuItemSaveInsole;
	m_menuItemSaveInsole = new wxMenuItem( m_menuGeometry, ID_INSOLE, wxString( _("Save &Insole") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuGeometry->Append( m_menuItemSaveInsole );
	m_menuItemSaveInsole->Enable( false );

	wxMenuItem* m_menuItemSaveHeel;
	m_menuItemSaveHeel = new wxMenuItem( m_menuGeometry, ID_HEEL, wxString( _("Save &Heel") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuGeometry->Append( m_menuItemSaveHeel );
	m_menuItemSaveHeel->Enable( false );

	wxMenuItem* m_menuItemCutaway;
	m_menuItemCutaway = new wxMenuItem( m_menuGeometry, ID_CUTAWAY, wxString( _("Save &Cutaway Object") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuGeometry->Append( m_menuItemCutaway );
	m_menuItemCutaway->Enable( false );

	m_menuGeometry->AppendSeparator();

	wxMenuItem* m_menuItemPackZip;
	m_menuItemPackZip = new wxMenuItem( m_menuGeometry, ID_ZIP, wxString( _("Pack a Zip with &Everything") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuGeometry->Append( m_menuItemPackZip );
	m_menuItemPackZip->Enable( false );

	m_menubar->Append( m_menuGeometry, _("&Geometry") );

	m_menuView = new wxMenu();
	wxMenuItem* m_menuItemStereo3D;
	m_menuItemStereo3D = new wxMenuItem( m_menuView, ID_STEREO3D, wxString( _("Toggle Stereo &3D") ) + wxT('\t') + wxT("CTRL+3"), _("Toggle 3D view on/off."), wxITEM_CHECK );
	m_menuView->Append( m_menuItemStereo3D );

	m_menuView->AppendSeparator();

	wxMenuItem* m_menuItemShowLeft;
	m_menuItemShowLeft = new wxMenuItem( m_menuView, ID_LEFT, wxString( _("Show &Left") ) + wxT('\t') + wxT("Ctrl+E"), wxEmptyString, wxITEM_CHECK );
	m_menuView->Append( m_menuItemShowLeft );

	wxMenuItem* m_menuItemShowRight;
	m_menuItemShowRight = new wxMenuItem( m_menuView, ID_RIGHT, wxString( _("Show &Right") ) + wxT('\t') + wxT("Ctrl+R"), wxEmptyString, wxITEM_CHECK );
	m_menuView->Append( m_menuItemShowRight );

	m_menuView->AppendSeparator();

	wxMenuItem* m_menuItemShowBones;
	m_menuItemShowBones = new wxMenuItem( m_menuView, ID_BONES, wxString( _("Show &Bones") ) , wxEmptyString, wxITEM_CHECK );
	m_menuView->Append( m_menuItemShowBones );

	wxMenuItem* m_menuItemShowSkin;
	m_menuItemShowSkin = new wxMenuItem( m_menuView, ID_SKIN, wxString( _("Show &Skin") ) , wxEmptyString, wxITEM_CHECK );
	m_menuView->Append( m_menuItemShowSkin );

	wxMenuItem* m_menuItemShowLeg;
	m_menuItemShowLeg = new wxMenuItem( m_menuView, ID_LEG, wxString( _("Show L&eg") ) , wxEmptyString, wxITEM_CHECK );
	m_menuView->Append( m_menuItemShowLeg );
	m_menuItemShowLeg->Enable( false );

	wxMenuItem* m_menuItemShowLast;
	m_menuItemShowLast = new wxMenuItem( m_menuView, ID_LAST, wxString( _("Show L&ast") ) , wxEmptyString, wxITEM_CHECK );
	m_menuView->Append( m_menuItemShowLast );

	wxMenuItem* m_menuItemShowInsole;
	m_menuItemShowInsole = new wxMenuItem( m_menuView, ID_INSOLE, wxString( _("Show &Insole") ) , wxEmptyString, wxITEM_CHECK );
	m_menuView->Append( m_menuItemShowInsole );

	wxMenuItem* m_menuIItemShowHeel;
	m_menuIItemShowHeel = new wxMenuItem( m_menuView, ID_HEEL, wxString( _("Show Heel") ) , wxEmptyString, wxITEM_CHECK );
	m_menuView->Append( m_menuIItemShowHeel );

	wxMenuItem* m_menuItemShowUpper;
	m_menuItemShowUpper = new wxMenuItem( m_menuView, ID_UPPER, wxString( _("Show &Upper") ) , wxEmptyString, wxITEM_CHECK );
	m_menuView->Append( m_menuItemShowUpper );

	wxMenuItem* m_menuItemShowCutaway;
	m_menuItemShowCutaway = new wxMenuItem( m_menuView, ID_CUTAWAY, wxString( _("Show &Cutaway Block") ) , wxEmptyString, wxITEM_CHECK );
	m_menuView->Append( m_menuItemShowCutaway );

	wxMenuItem* m_menuItemShowFloor;
	m_menuItemShowFloor = new wxMenuItem( m_menuView, ID_FLOOR, wxString( _("Show &Floor") ) , wxEmptyString, wxITEM_CHECK );
	m_menuView->Append( m_menuItemShowFloor );

	m_menuView->AppendSeparator();

	wxMenuItem* m_menuItemShowCoordinateSystem;
	m_menuItemShowCoordinateSystem = new wxMenuItem( m_menuView, ID_COORDINATESYSTEM, wxString( _("Show Coor&dinate System") ) , wxEmptyString, wxITEM_CHECK );
	m_menuView->Append( m_menuItemShowCoordinateSystem );

	wxMenuItem* m_menuItemShowBackground;
	m_menuItemShowBackground = new wxMenuItem( m_menuView, ID_BACKGROUND, wxString( _("Show Back&ground") ) + wxT('\t') + wxT("CTRL+B"), wxEmptyString, wxITEM_CHECK );
	m_menuView->Append( m_menuItemShowBackground );

	m_menuView->AppendSeparator();

	wxMenuItem* m_menuItemSetupBackground;
	m_menuItemSetupBackground = new wxMenuItem( m_menuView, ID_BACKGROUND_SETUP, wxString( _("Setup Background Images") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuView->Append( m_menuItemSetupBackground );

	m_menubar->Append( m_menuView, _("&View") );

	m_menuPreferences = new wxMenu();
	m_menubar->Append( m_menuPreferences, _("P&references") );

	m_menuHelp = new wxMenu();
	wxMenuItem* m_menuItemCalculator;
	m_menuItemCalculator = new wxMenuItem( m_menuHelp, ID_CALCULATOR, wxString( _("Calculator") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuHelp->Append( m_menuItemCalculator );

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

	m_radioBtnEditLeft = new wxRadioButton( m_panelPageFoot, ID_LEFT, _("Edit Left"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	bSizerEditLeftRight->Add( m_radioBtnEditLeft, 1, wxALL, 5 );

	m_radioBtnEditBoth = new wxRadioButton( m_panelPageFoot, ID_BOTH, _("Edit Both"), wxDefaultPosition, wxDefaultSize, 0 );
	m_radioBtnEditBoth->SetValue( true );
	bSizerEditLeftRight->Add( m_radioBtnEditBoth, 1, wxALL, 5 );

	m_radioBtnEditRight = new wxRadioButton( m_panelPageFoot, ID_RIGHT, _("Edit Right"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerEditLeftRight->Add( m_radioBtnEditRight, 1, wxALL, 5 );


	bSizerFoot->Add( bSizerEditLeftRight, 0, wxEXPAND, 5 );

	m_choicebookMeasurementSource = new wxChoicebook( m_panelPageFoot, ID_MEASUREMENTSOURCE, wxDefaultPosition, wxDefaultSize, wxCHB_DEFAULT );
	m_panelMeasurementBased = new wxPanel( m_choicebookMeasurementSource, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerFootMeasurements;
	bSizerFootMeasurements = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizerFootMeasurementsHorizontal;
	bSizerFootMeasurementsHorizontal = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizerFootMeasurementsVertical;
	bSizerFootMeasurementsVertical = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizerFootMeasurements;
	fgSizerFootMeasurements = new wxFlexGridSizer( 8, 2, 0, 0 );
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

	m_staticTextLegLengthDifference = new wxStaticText( m_panelMeasurementBased, wxID_ANY, _("Leg length difference:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextLegLengthDifference->Wrap( -1 );
	fgSizerFootMeasurements->Add( m_staticTextLegLengthDifference, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_textCtrlLegLengthDifference = new ExtendedTextCtrl( m_panelMeasurementBased, ID_MEASUREMENT_LEGLENGTHDIFFERENCE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerFootMeasurements->Add( m_textCtrlLegLengthDifference, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );


	bSizerFootMeasurementsVertical->Add( fgSizerFootMeasurements, 0, wxEXPAND, 5 );


	bSizerFootMeasurementsHorizontal->Add( bSizerFootMeasurementsVertical, 1, 0, 5 );

	wxBoxSizer* bSizerShoeSizes;
	bSizerShoeSizes = new wxBoxSizer( wxVERTICAL );

	m_buttonQuickSetup = new wxButton( m_panelMeasurementBased, ID_MEASUREMENTS_SETUP, _("Quick Setup"), wxDefaultPosition, wxDefaultSize, 0 );
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
	m_choicebookMeasurementSource->AddPage( m_panelMeasurementBased, _("Measurement based"), true );
	m_panelScanBased = new wxPanel( m_choicebookMeasurementSource, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerScanBasedModel;
	bSizerScanBasedModel = new wxBoxSizer( wxVERTICAL );

	m_staticTextScanFile = new wxStaticText( m_panelScanBased, wxID_ANY, _("Scan file:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextScanFile->Wrap( -1 );
	bSizerScanBasedModel->Add( m_staticTextScanFile, 0, wxALL, 5 );

	m_filePickerScan = new wxFilePickerCtrl( m_panelScanBased, ID_SCAN, wxEmptyString, _("Select a file"), _("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE|wxFLP_FILE_MUST_EXIST|wxFLP_OPEN|wxFLP_USE_TEXTCTRL );
	bSizerScanBasedModel->Add( m_filePickerScan, 0, wxALL|wxEXPAND, 5 );


	m_panelScanBased->SetSizer( bSizerScanBasedModel );
	m_panelScanBased->Layout();
	bSizerScanBasedModel->Fit( m_panelScanBased );
	m_choicebookMeasurementSource->AddPage( m_panelScanBased, _("Scan based"), false );
	bSizerFoot->Add( m_choicebookMeasurementSource, 1, wxEXPAND | wxALL, 5 );


	m_panelPageFoot->SetSizer( bSizerFoot );
	m_panelPageFoot->Layout();
	bSizerFoot->Fit( m_panelPageFoot );
	m_notebook->AddPage( m_panelPageFoot, _("Foot"), true );
	m_panelPageLeg = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerLeg;
	bSizerLeg = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizerLeftRight1;
	bSizerLeftRight1 = new wxBoxSizer( wxHORIZONTAL );

	m_radioBtnEditLegLeft = new wxRadioButton( m_panelPageLeg, ID_LEFT, _("Edit Left"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	bSizerLeftRight1->Add( m_radioBtnEditLegLeft, 1, wxALL, 5 );

	m_radioBtnEditLegBoth = new wxRadioButton( m_panelPageLeg, ID_BOTH, _("Edit Both"), wxDefaultPosition, wxDefaultSize, 0 );
	m_radioBtnEditLegBoth->SetValue( true );
	bSizerLeftRight1->Add( m_radioBtnEditLegBoth, 1, wxALL, 5 );

	m_radioBtnEditLegRight = new wxRadioButton( m_panelPageLeg, ID_RIGHT, _("Edit Right"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerLeftRight1->Add( m_radioBtnEditLegRight, 1, wxALL, 5 );


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
	m_panelPageLast = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerLast;
	bSizerLast = new wxBoxSizer( wxVERTICAL );

	m_choicebookLastConstructionType = new wxChoicebook( m_panelPageLast, ID_LASTCONSTRUCTIONTYPE, wxDefaultPosition, wxDefaultSize, wxCHB_DEFAULT );
	m_panelLastConstruct = new wxPanel( m_choicebookLastConstructionType, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* sbSizerShapeParameter;
	sbSizerShapeParameter = new wxStaticBoxSizer( new wxStaticBox( m_panelLastConstruct, wxID_ANY, _("Insole shape") ), wxVERTICAL );

	wxFlexGridSizer* fgSizerShapePatameter;
	fgSizerShapePatameter = new wxFlexGridSizer( 8, 2, 0, 0 );
	fgSizerShapePatameter->AddGrowableCol( 1 );
	fgSizerShapePatameter->SetFlexibleDirection( wxBOTH );
	fgSizerShapePatameter->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextBigToeAngle = new wxStaticText( sbSizerShapeParameter->GetStaticBox(), wxID_ANY, _("Big toe angle:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextBigToeAngle->Wrap( -1 );
	fgSizerShapePatameter->Add( m_staticTextBigToeAngle, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_textCtrlBigToeAngle = new ExtendedTextCtrl( sbSizerShapeParameter->GetStaticBox(), ID_BIGTOEANGLE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerShapePatameter->Add( m_textCtrlBigToeAngle, 0, wxALL|wxEXPAND, 5 );

	m_staticTextLittleToeAngle = new wxStaticText( sbSizerShapeParameter->GetStaticBox(), wxID_ANY, _("Little toe angle:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextLittleToeAngle->Wrap( -1 );
	fgSizerShapePatameter->Add( m_staticTextLittleToeAngle, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_textCtrlLittleToeAngle = new ExtendedTextCtrl( sbSizerShapeParameter->GetStaticBox(), ID_LITTLETOEANGLE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerShapePatameter->Add( m_textCtrlLittleToeAngle, 0, wxALL|wxEXPAND, 5 );

	m_staticTextBallMeasurementAngle = new wxStaticText( sbSizerShapeParameter->GetStaticBox(), wxID_ANY, _("Ball measurement angle:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextBallMeasurementAngle->Wrap( -1 );
	fgSizerShapePatameter->Add( m_staticTextBallMeasurementAngle, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_textCtrlBallMeasurementAngle = new ExtendedTextCtrl( sbSizerShapeParameter->GetStaticBox(), ID_BALLMEASUREMENTANGLE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerShapePatameter->Add( m_textCtrlBallMeasurementAngle, 0, wxALL|wxEXPAND, 5 );

	m_staticTextHeelDirectionAngle = new wxStaticText( sbSizerShapeParameter->GetStaticBox(), wxID_ANY, _("Heel direction angle:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextHeelDirectionAngle->Wrap( -1 );
	fgSizerShapePatameter->Add( m_staticTextHeelDirectionAngle, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_textCtrlHeelDirectionAngle = new ExtendedTextCtrl( sbSizerShapeParameter->GetStaticBox(), ID_HEELDIRECTIONANGLE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerShapePatameter->Add( m_textCtrlHeelDirectionAngle, 0, wxALL|wxEXPAND, 5 );

	m_staticTextTipSharpness = new wxStaticText( sbSizerShapeParameter->GetStaticBox(), wxID_ANY, _("Tip sharpness:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextTipSharpness->Wrap( -1 );
	fgSizerShapePatameter->Add( m_staticTextTipSharpness, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_textCtrlTipSharpness = new ExtendedTextCtrl( sbSizerShapeParameter->GetStaticBox(), ID_TIPSHARPNESS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerShapePatameter->Add( m_textCtrlTipSharpness, 0, wxALL|wxEXPAND, 5 );

	m_staticTextAngleMixing = new wxStaticText( sbSizerShapeParameter->GetStaticBox(), wxID_ANY, _("Angle Mixing:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextAngleMixing->Wrap( -1 );
	fgSizerShapePatameter->Add( m_staticTextAngleMixing, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_textCtrlAngleMixing = new ExtendedTextCtrl( sbSizerShapeParameter->GetStaticBox(), ID_MEASUREMENT_ANGLEMIXING, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerShapePatameter->Add( m_textCtrlAngleMixing, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );

	m_staticTextExtraLength = new wxStaticText( sbSizerShapeParameter->GetStaticBox(), wxID_ANY, _("Extra length:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextExtraLength->Wrap( -1 );
	fgSizerShapePatameter->Add( m_staticTextExtraLength, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_textCtrlExtraLength = new ExtendedTextCtrl( sbSizerShapeParameter->GetStaticBox(), ID_EXTRALENGTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerShapePatameter->Add( m_textCtrlExtraLength, 0, wxALL|wxEXPAND, 5 );

	m_staticTextFootCompression = new wxStaticText( sbSizerShapeParameter->GetStaticBox(), wxID_ANY, _("Foot compression:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextFootCompression->Wrap( -1 );
	fgSizerShapePatameter->Add( m_staticTextFootCompression, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_textCtrlFootCompression = new ExtendedTextCtrl( sbSizerShapeParameter->GetStaticBox(), ID_FOOTCOMPRESSION, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerShapePatameter->Add( m_textCtrlFootCompression, 0, wxALL|wxEXPAND, 5 );


	sbSizerShapeParameter->Add( fgSizerShapePatameter, 1, wxEXPAND, 5 );


	m_panelLastConstruct->SetSizer( sbSizerShapeParameter );
	m_panelLastConstruct->Layout();
	sbSizerShapeParameter->Fit( m_panelLastConstruct );
	m_choicebookLastConstructionType->AddPage( m_panelLastConstruct, _("Constructed last"), true );
	m_panelLastBoneBased = new wxPanel( m_choicebookLastConstructionType, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerBoneBasedModel;
	bSizerBoneBasedModel = new wxBoxSizer( wxVERTICAL );

	m_staticTextBondeModel = new wxStaticText( m_panelLastBoneBased, wxID_ANY, _("Bone Model:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextBondeModel->Wrap( -1 );
	bSizerBoneBasedModel->Add( m_staticTextBondeModel, 0, wxALL, 5 );

	m_filePickerBoneModel = new wxFilePickerCtrl( m_panelLastBoneBased, ID_BONES, wxEmptyString, _("Select a file"), _("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE|wxFLP_FILE_MUST_EXIST|wxFLP_OPEN|wxFLP_USE_TEXTCTRL );
	bSizerBoneBasedModel->Add( m_filePickerBoneModel, 0, wxALL|wxEXPAND, 5 );

	m_buttonEditBoneModel = new wxButton( m_panelLastBoneBased, ID_BONES_EDIT, _("Edit bone model"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerBoneBasedModel->Add( m_buttonEditBoneModel, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );


	m_panelLastBoneBased->SetSizer( bSizerBoneBasedModel );
	m_panelLastBoneBased->Layout();
	bSizerBoneBasedModel->Fit( m_panelLastBoneBased );
	m_choicebookLastConstructionType->AddPage( m_panelLastBoneBased, _("Bone based last model"), false );
	m_panelLastLoadFromFile = new wxPanel( m_choicebookLastConstructionType, ID_HEELREORIENT, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerLastBasedModel;
	bSizerLastBasedModel = new wxBoxSizer( wxVERTICAL );

	m_staticTextLastModel = new wxStaticText( m_panelLastLoadFromFile, wxID_ANY, _("Last Model:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextLastModel->Wrap( -1 );
	bSizerLastBasedModel->Add( m_staticTextLastModel, 0, wxALL, 5 );

	m_filePickerLast = new wxFilePickerCtrl( m_panelLastLoadFromFile, ID_LAST, wxEmptyString, _("Select last model"), _("3D Models (*.stl, *.obj)|*.stl; *.obj|Stereolitography models(*.stl)|*.stl|Wavefront objects (*.obj)|*.obj|All files|*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE|wxFLP_FILE_MUST_EXIST|wxFLP_OPEN|wxFLP_USE_TEXTCTRL );
	bSizerLastBasedModel->Add( m_filePickerLast, 0, wxALL|wxEXPAND, 5 );

	m_checkBoxLastModify = new wxCheckBox( m_panelLastLoadFromFile, ID_LASTMODIFY, _("Modify/Normalize Last"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBoxLastModify->SetValue(true);
	bSizerLastBasedModel->Add( m_checkBoxLastModify, 0, wxALL, 5 );

	m_checkBoxLastReorient = new wxCheckBox( m_panelLastLoadFromFile, ID_LASTREORIENT, _("Reorient Last"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBoxLastReorient->SetValue(true);
	bSizerLastBasedModel->Add( m_checkBoxLastReorient, 0, wxALL, 5 );


	m_panelLastLoadFromFile->SetSizer( bSizerLastBasedModel );
	m_panelLastLoadFromFile->Layout();
	bSizerLastBasedModel->Fit( m_panelLastLoadFromFile );
	m_choicebookLastConstructionType->AddPage( m_panelLastLoadFromFile, _("Load last from file"), false );
	bSizerLast->Add( m_choicebookLastConstructionType, 0, wxALL|wxEXPAND, 5 );

	m_choicebookHeelConstructionType = new wxChoicebook( m_panelPageLast, ID_HEELCONSTRUCTIONTYPE, wxDefaultPosition, wxDefaultSize, wxCHB_DEFAULT );
	m_panelHeelFromCode = new wxPanel( m_choicebookHeelConstructionType, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerHeelFromCode;
	bSizerHeelFromCode = new wxBoxSizer( wxVERTICAL );

	m_staticTextHeelFromCode = new wxStaticText( m_panelHeelFromCode, wxID_ANY, _("(see Tab \"Heel\")"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextHeelFromCode->Wrap( -1 );
	bSizerHeelFromCode->Add( m_staticTextHeelFromCode, 0, wxALL, 5 );


	m_panelHeelFromCode->SetSizer( bSizerHeelFromCode );
	m_panelHeelFromCode->Layout();
	bSizerHeelFromCode->Fit( m_panelHeelFromCode );
	m_choicebookHeelConstructionType->AddPage( m_panelHeelFromCode, _("Construct heel from code"), false );
	m_panelHeelFromFile = new wxPanel( m_choicebookHeelConstructionType, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerHeelFromFile;
	bSizerHeelFromFile = new wxBoxSizer( wxVERTICAL );

	m_staticTextHeel = new wxStaticText( m_panelHeelFromFile, wxID_ANY, _("Heel:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextHeel->Wrap( -1 );
	bSizerHeelFromFile->Add( m_staticTextHeel, 0, wxALL, 5 );

	m_filePickerHeel = new wxFilePickerCtrl( m_panelHeelFromFile, ID_HEEL, wxEmptyString, _("Select a file"), _("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE|wxFLP_FILE_MUST_EXIST|wxFLP_OPEN|wxFLP_USE_TEXTCTRL );
	bSizerHeelFromFile->Add( m_filePickerHeel, 0, wxALL|wxEXPAND, 5 );

	m_checkBoxHeelReorient = new wxCheckBox( m_panelHeelFromFile, ID_HEELREORIENT, _("Reorient heel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBoxHeelReorient->SetValue(true);
	bSizerHeelFromFile->Add( m_checkBoxHeelReorient, 0, wxALL, 5 );


	m_panelHeelFromFile->SetSizer( bSizerHeelFromFile );
	m_panelHeelFromFile->Layout();
	bSizerHeelFromFile->Fit( m_panelHeelFromFile );
	m_choicebookHeelConstructionType->AddPage( m_panelHeelFromFile, _("Load heel from file"), true );
	bSizerLast->Add( m_choicebookHeelConstructionType, 1, wxEXPAND | wxALL, 5 );


	m_panelPageLast->SetSizer( bSizerLast );
	m_panelPageLast->Layout();
	bSizerLast->Fit( m_panelPageLast );
	m_notebook->AddPage( m_panelPageLast, _("Last"), false );
	m_panelPageShoe = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerShoe;
	bSizerShoe = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* sbSizerHeelStyle;
	sbSizerHeelStyle = new wxStaticBoxSizer( new wxStaticBox( m_panelPageShoe, wxID_ANY, _("Heel style") ), wxVERTICAL );

	wxArrayString m_choicePresetShoeTypeChoices;
	m_choicePresetShoeType = new wxChoice( sbSizerHeelStyle->GetStaticBox(), ID_PRESETSHOETYPE, wxDefaultPosition, wxDefaultSize, m_choicePresetShoeTypeChoices, 0 );
	m_choicePresetShoeType->SetSelection( 0 );
	sbSizerHeelStyle->Add( m_choicePresetShoeType, 0, wxALL|wxEXPAND, 5 );

	wxFlexGridSizer* fgSizerShoeBaseMeasurements;
	fgSizerShoeBaseMeasurements = new wxFlexGridSizer( 4, 2, 0, 0 );
	fgSizerShoeBaseMeasurements->AddGrowableCol( 1 );
	fgSizerShoeBaseMeasurements->SetFlexibleDirection( wxHORIZONTAL );
	fgSizerShoeBaseMeasurements->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextHeelHeight = new wxStaticText( sbSizerHeelStyle->GetStaticBox(), wxID_ANY, _("Heel Height:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextHeelHeight->Wrap( -1 );
	fgSizerShoeBaseMeasurements->Add( m_staticTextHeelHeight, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_textCtrlHeelHeight = new ExtendedTextCtrl( sbSizerHeelStyle->GetStaticBox(), ID_HEELHEIGHT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerShoeBaseMeasurements->Add( m_textCtrlHeelHeight, 0, wxALL|wxEXPAND, 5 );

	m_staticTextBallHeight = new wxStaticText( sbSizerHeelStyle->GetStaticBox(), wxID_ANY, _("Ball Height:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextBallHeight->Wrap( -1 );
	fgSizerShoeBaseMeasurements->Add( m_staticTextBallHeight, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_textCtrlBallHeight = new ExtendedTextCtrl( sbSizerHeelStyle->GetStaticBox(), ID_BALLHEIGHT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerShoeBaseMeasurements->Add( m_textCtrlBallHeight, 0, wxALL|wxEXPAND, 5 );

	m_staticTextHeelPitch = new wxStaticText( sbSizerHeelStyle->GetStaticBox(), wxID_ANY, _("Heel Pitch:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextHeelPitch->Wrap( -1 );
	fgSizerShoeBaseMeasurements->Add( m_staticTextHeelPitch, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrlHeelPitch = new ExtendedTextCtrl( sbSizerHeelStyle->GetStaticBox(), ID_HEELPITCH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerShoeBaseMeasurements->Add( m_textCtrlHeelPitch, 0, wxALL|wxEXPAND, 5 );

	m_staticTextToeSpring = new wxStaticText( sbSizerHeelStyle->GetStaticBox(), wxID_ANY, _("Toe Spring:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextToeSpring->Wrap( -1 );
	fgSizerShoeBaseMeasurements->Add( m_staticTextToeSpring, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_textCtrlToeSpring = new ExtendedTextCtrl( sbSizerHeelStyle->GetStaticBox(), ID_TOESPRING, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerShoeBaseMeasurements->Add( m_textCtrlToeSpring, 0, wxALL|wxEXPAND, 5 );


	sbSizerHeelStyle->Add( fgSizerShoeBaseMeasurements, 0, wxEXPAND, 5 );


	bSizerShoe->Add( sbSizerHeelStyle, 0, wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizerShoeStyle;
	sbSizerShoeStyle = new wxStaticBoxSizer( new wxStaticBox( m_panelPageShoe, wxID_ANY, _("Shoe style") ), wxVERTICAL );

	wxArrayString m_choicePresetShoeStyleChoices;
	m_choicePresetShoeStyle = new wxChoice( sbSizerShoeStyle->GetStaticBox(), ID_PRESETSHOEHEIGHT, wxDefaultPosition, wxDefaultSize, m_choicePresetShoeStyleChoices, 0 );
	m_choicePresetShoeStyle->SetSelection( 1 );
	sbSizerShoeStyle->Add( m_choicePresetShoeStyle, 0, wxALL|wxEXPAND, 5 );

	wxFlexGridSizer* fgSizerUpperLevel;
	fgSizerUpperLevel = new wxFlexGridSizer( 1, 2, 0, 0 );
	fgSizerUpperLevel->AddGrowableCol( 1 );
	fgSizerUpperLevel->SetFlexibleDirection( wxHORIZONTAL );
	fgSizerUpperLevel->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextUpperLevel = new wxStaticText( sbSizerShoeStyle->GetStaticBox(), wxID_ANY, _("Upper level:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextUpperLevel->Wrap( -1 );
	fgSizerUpperLevel->Add( m_staticTextUpperLevel, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_textCtrlUpperLevel = new ExtendedTextCtrl( sbSizerShoeStyle->GetStaticBox(), ID_UPPERLEVEL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerUpperLevel->Add( m_textCtrlUpperLevel, 0, wxALL|wxEXPAND, 5 );


	sbSizerShoeStyle->Add( fgSizerUpperLevel, 0, wxEXPAND, 5 );


	bSizerShoe->Add( sbSizerShoeStyle, 0, wxEXPAND, 5 );

	m_staticTextConstruction = new wxStaticText( m_panelPageShoe, wxID_ANY, _("Construction:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextConstruction->Wrap( -1 );
	bSizerShoe->Add( m_staticTextConstruction, 0, wxALL, 5 );

	m_choicebookConstruction = new wxChoicebook( m_panelPageShoe, ID_GENERATOR, wxDefaultPosition, wxDefaultSize, wxCHB_DEFAULT );
	m_panelExperimental = new wxPanel( m_choicebookConstruction, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_choicebookConstruction->AddPage( m_panelExperimental, _("Experimental Generator"), true );
	m_panelWelted = new wxPanel( m_choicebookConstruction, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizerWelted;
	fgSizerWelted = new wxFlexGridSizer( 1, 2, 0, 0 );
	fgSizerWelted->AddGrowableCol( 1 );
	fgSizerWelted->SetFlexibleDirection( wxBOTH );
	fgSizerWelted->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextWeltSize = new wxStaticText( m_panelWelted, wxID_ANY, _("Welt size:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextWeltSize->Wrap( -1 );
	fgSizerWelted->Add( m_staticTextWeltSize, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrlWeltSize = new ExtendedTextCtrl( m_panelWelted, ID_WELTSIZE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerWelted->Add( m_textCtrlWeltSize, 0, wxALL|wxEXPAND, 5 );


	m_panelWelted->SetSizer( fgSizerWelted );
	m_panelWelted->Layout();
	fgSizerWelted->Fit( m_panelWelted );
	m_choicebookConstruction->AddPage( m_panelWelted, _("Welted Construction"), false );
	m_panelCemented = new wxPanel( m_choicebookConstruction, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_choicebookConstruction->AddPage( m_panelCemented, _("Cemented Sole"), false );
	m_panelMolded = new wxPanel( m_choicebookConstruction, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_choicebookConstruction->AddPage( m_panelMolded, _("Molded Sole"), false );
	m_panelClog = new wxPanel( m_choicebookConstruction, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerClog;
	bSizerClog = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer9;
	fgSizer9 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer9->AddGrowableCol( 1 );
	fgSizer9->SetFlexibleDirection( wxBOTH );
	fgSizer9->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextThickness = new wxStaticText( m_panelClog, wxID_ANY, _("Thickness:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextThickness->Wrap( -1 );
	fgSizer9->Add( m_staticTextThickness, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrlThickness = new ExtendedTextCtrl( m_panelClog, ID_THICKNESS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizer9->Add( m_textCtrlThickness, 0, wxALL|wxEXPAND, 5 );


	bSizerClog->Add( fgSizer9, 0, wxEXPAND, 5 );


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

	m_buttonAddBridge = new wxButton( m_panelGeta, ID_BRIDGE, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerGetaButtons->Add( m_buttonAddBridge, 0, wxALL, 5 );

	m_buttonDeleteBridge = new wxButton( m_panelGeta, ID_BRIDGE, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
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
	bSizerShoe->Add( m_choicebookConstruction, 1, wxALL|wxEXPAND, 5 );


	m_panelPageShoe->SetSizer( bSizerShoe );
	m_panelPageShoe->Layout();
	bSizerShoe->Fit( m_panelPageShoe );
	m_notebook->AddPage( m_panelPageShoe, _("Shoe"), false );
	m_panelPageHeel = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerHeel;
	bSizerHeel = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* sbSizerWalkcycleSupport;
	sbSizerWalkcycleSupport = new wxStaticBoxSizer( new wxStaticBox( m_panelPageHeel, wxID_ANY, _("Walkcycle Support Heel") ), wxVERTICAL );

	wxFlexGridSizer* fgSizerWalkcycleSupport;
	fgSizerWalkcycleSupport = new wxFlexGridSizer( 4, 2, 0, 0 );
	fgSizerWalkcycleSupport->AddGrowableCol( 1 );
	fgSizerWalkcycleSupport->SetFlexibleDirection( wxBOTH );
	fgSizerWalkcycleSupport->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextSupportHeelRadius = new wxStaticText( sbSizerWalkcycleSupport->GetStaticBox(), wxID_ANY, _("Support Heel Radius:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextSupportHeelRadius->Wrap( -1 );
	fgSizerWalkcycleSupport->Add( m_staticTextSupportHeelRadius, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrlSupportHeelRadius = new ExtendedTextCtrl( sbSizerWalkcycleSupport->GetStaticBox(), ID_SUPPORTHEELRADIUS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerWalkcycleSupport->Add( m_textCtrlSupportHeelRadius, 0, wxALL|wxEXPAND, 5 );

	m_staticTextSupportHeelOffset = new wxStaticText( sbSizerWalkcycleSupport->GetStaticBox(), wxID_ANY, _("Support Heel Offset:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextSupportHeelOffset->Wrap( -1 );
	fgSizerWalkcycleSupport->Add( m_staticTextSupportHeelOffset, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrlSupportHeelOffset = new ExtendedTextCtrl( sbSizerWalkcycleSupport->GetStaticBox(), ID_SUPPORTHEELOFFSET, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerWalkcycleSupport->Add( m_textCtrlSupportHeelOffset, 0, wxALL|wxEXPAND, 5 );

	m_staticTextSupportToeRadius = new wxStaticText( sbSizerWalkcycleSupport->GetStaticBox(), wxID_ANY, _("Support Toe Radius:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextSupportToeRadius->Wrap( -1 );
	fgSizerWalkcycleSupport->Add( m_staticTextSupportToeRadius, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrlSupportToeRadius = new ExtendedTextCtrl( sbSizerWalkcycleSupport->GetStaticBox(), ID_SUPPORTTOERADIUS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerWalkcycleSupport->Add( m_textCtrlSupportToeRadius, 0, wxALL|wxEXPAND, 5 );

	m_staticTextSupportToeOffset = new wxStaticText( sbSizerWalkcycleSupport->GetStaticBox(), wxID_ANY, _("Support Toe Offset:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextSupportToeOffset->Wrap( -1 );
	fgSizerWalkcycleSupport->Add( m_staticTextSupportToeOffset, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrlSupportToeOffset = new ExtendedTextCtrl( sbSizerWalkcycleSupport->GetStaticBox(), ID_SUPPORTTOEOFFSET, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizerWalkcycleSupport->Add( m_textCtrlSupportToeOffset, 0, wxALL|wxEXPAND, 5 );


	sbSizerWalkcycleSupport->Add( fgSizerWalkcycleSupport, 0, wxEXPAND, 5 );


	bSizerHeel->Add( sbSizerWalkcycleSupport, 0, wxEXPAND, 5 );

	m_staticTextHeelPreset = new wxStaticText( m_panelPageHeel, wxID_ANY, _("Preset:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextHeelPreset->Wrap( -1 );
	bSizerHeel->Add( m_staticTextHeelPreset, 0, wxALL, 5 );

	wxArrayString m_choicePresetHeelChoices;
	m_choicePresetHeel = new wxChoice( m_panelPageHeel, ID_PRESETHEEL, wxDefaultPosition, wxDefaultSize, m_choicePresetHeelChoices, 0 );
	m_choicePresetHeel->SetSelection( 0 );
	bSizerHeel->Add( m_choicePresetHeel, 0, wxALL|wxEXPAND, 5 );

	wxFlexGridSizer* fgSizerHeelCode;
	fgSizerHeelCode = new wxFlexGridSizer( 3, 2, 0, 0 );
	fgSizerHeelCode->AddGrowableCol( 0 );
	fgSizerHeelCode->AddGrowableCol( 1 );
	fgSizerHeelCode->AddGrowableRow( 2 );
	fgSizerHeelCode->SetFlexibleDirection( wxBOTH );
	fgSizerHeelCode->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextHeelCode = new wxStaticText( m_panelPageHeel, wxID_ANY, _("Code:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextHeelCode->Wrap( -1 );
	fgSizerHeelCode->Add( m_staticTextHeelCode, 0, wxALL, 5 );

	m_staticTextHeelVariant = new wxStaticText( m_panelPageHeel, wxID_ANY, _("Variant:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextHeelVariant->Wrap( -1 );
	fgSizerHeelCode->Add( m_staticTextHeelVariant, 0, wxALL, 5 );

	wxArrayString m_choicePresetHeelCodeChoices;
	m_choicePresetHeelCode = new wxChoice( m_panelPageHeel, ID_PRESETHEELCODE, wxDefaultPosition, wxDefaultSize, m_choicePresetHeelCodeChoices, 0 );
	m_choicePresetHeelCode->SetSelection( 0 );
	fgSizerHeelCode->Add( m_choicePresetHeelCode, 0, wxALL|wxEXPAND, 5 );

	wxArrayString m_choicePresetHeelVariantChoices;
	m_choicePresetHeelVariant = new wxChoice( m_panelPageHeel, ID_PRESETHEELVARIANT, wxDefaultPosition, wxDefaultSize, m_choicePresetHeelVariantChoices, 0 );
	m_choicePresetHeelVariant->SetSelection( 0 );
	fgSizerHeelCode->Add( m_choicePresetHeelVariant, 0, wxALL|wxEXPAND, 5 );

	m_editorCode = new wxStyledTextCtrl( m_panelPageHeel, ID_HEELCODE, wxDefaultPosition, wxDefaultSize, 0, wxEmptyString );
	m_editorCode->SetUseTabs( true );
	m_editorCode->SetTabWidth( 4 );
	m_editorCode->SetIndent( 4 );
	m_editorCode->SetTabIndents( true );
	m_editorCode->SetBackSpaceUnIndents( true );
	m_editorCode->SetViewEOL( false );
	m_editorCode->SetViewWhiteSpace( false );
	m_editorCode->SetMarginWidth( 2, 0 );
	m_editorCode->SetIndentationGuides( true );
	m_editorCode->SetReadOnly( false );
	m_editorCode->SetMarginWidth( 1, 0 );
	m_editorCode->SetMarginType( 0, wxSTC_MARGIN_NUMBER );
	m_editorCode->SetMarginWidth( 0, m_editorCode->TextWidth( wxSTC_STYLE_LINENUMBER, wxT("_99999") ) );
	m_editorCode->MarkerDefine( wxSTC_MARKNUM_FOLDER, wxSTC_MARK_BOXPLUS );
	m_editorCode->MarkerSetBackground( wxSTC_MARKNUM_FOLDER, wxColour( wxT("BLACK") ) );
	m_editorCode->MarkerSetForeground( wxSTC_MARKNUM_FOLDER, wxColour( wxT("WHITE") ) );
	m_editorCode->MarkerDefine( wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_BOXMINUS );
	m_editorCode->MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPEN, wxColour( wxT("BLACK") ) );
	m_editorCode->MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPEN, wxColour( wxT("WHITE") ) );
	m_editorCode->MarkerDefine( wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_EMPTY );
	m_editorCode->MarkerDefine( wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_BOXPLUS );
	m_editorCode->MarkerSetBackground( wxSTC_MARKNUM_FOLDEREND, wxColour( wxT("BLACK") ) );
	m_editorCode->MarkerSetForeground( wxSTC_MARKNUM_FOLDEREND, wxColour( wxT("WHITE") ) );
	m_editorCode->MarkerDefine( wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_BOXMINUS );
	m_editorCode->MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPENMID, wxColour( wxT("BLACK") ) );
	m_editorCode->MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPENMID, wxColour( wxT("WHITE") ) );
	m_editorCode->MarkerDefine( wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY );
	m_editorCode->MarkerDefine( wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_EMPTY );
	m_editorCode->SetSelBackground( true, wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );
	m_editorCode->SetSelForeground( true, wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHTTEXT ) );
	fgSizerHeelCode->Add( m_editorCode, 1, wxEXPAND | wxALL, 5 );

	m_dataViewListCtrlVariant = new wxDataViewListCtrl( m_panelPageHeel, ID_HEELVARIANT, wxDefaultPosition, wxDefaultSize, wxDV_HORIZ_RULES|wxDV_SINGLE|wxDV_VERT_RULES );
	m_dataViewListColumnVariantName = m_dataViewListCtrlVariant->AppendTextColumn( _("Name"), wxDATAVIEW_CELL_INERT, -1, static_cast<wxAlignment>(wxALIGN_LEFT), wxDATAVIEW_COL_RESIZABLE );
	m_dataViewListColumnVariantName->GetRenderer()->EnableEllipsize( wxELLIPSIZE_END );
	m_dataViewListColumnVariantValue = m_dataViewListCtrlVariant->AppendTextColumn( _("Value"), wxDATAVIEW_CELL_EDITABLE, -1, static_cast<wxAlignment>(wxALIGN_LEFT), wxDATAVIEW_COL_RESIZABLE );
	m_dataViewListColumnVariantValue->GetRenderer()->EnableEllipsize( wxELLIPSIZE_NONE );
	fgSizerHeelCode->Add( m_dataViewListCtrlVariant, 0, wxALL|wxEXPAND, 5 );


	bSizerHeel->Add( fgSizerHeelCode, 1, wxEXPAND, 5 );


	m_panelPageHeel->SetSizer( bSizerHeel );
	m_panelPageHeel->Layout();
	bSizerHeel->Fit( m_panelPageHeel );
	m_notebook->AddPage( m_panelPageHeel, _("Heel"), false );
	m_panelPagePattern = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerPattern;
	bSizerPattern = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizerPatternLeft;
	bSizerPatternLeft = new wxBoxSizer( wxVERTICAL );

	m_treeListCtrlPattern = new wxTreeListCtrl( m_panelPagePattern, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTL_MULTIPLE );

	bSizerPatternLeft->Add( m_treeListCtrlPattern, 1, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer68;
	bSizer68 = new wxBoxSizer( wxHORIZONTAL );


	bSizer68->Add( 0, 0, 1, wxEXPAND, 5 );

	m_buttonElementCopy = new wxButton( m_panelPagePattern, ID_SELECTED, _("Copy"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer68->Add( m_buttonElementCopy, 0, wxALL, 5 );

	m_buttonElementDelete = new wxButton( m_panelPagePattern, ID_SELECTED, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer68->Add( m_buttonElementDelete, 0, wxALL, 5 );


	bSizer68->Add( 0, 0, 1, wxEXPAND, 5 );


	bSizerPatternLeft->Add( bSizer68, 0, wxEXPAND, 5 );


	bSizerPattern->Add( bSizerPatternLeft, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizerPatternRight;
	bSizerPatternRight = new wxBoxSizer( wxVERTICAL );

	m_buttonOnPatternAddLine = new wxButton( m_panelPagePattern, ID_LINE, _("Add Line"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerPatternRight->Add( m_buttonOnPatternAddLine, 0, wxALL|wxEXPAND, 5 );

	m_buttonOnPatternAddPatch = new wxButton( m_panelPagePattern, ID_PATCH, _("Add Patch"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerPatternRight->Add( m_buttonOnPatternAddPatch, 0, wxALL|wxEXPAND, 5 );

	m_buttonOnPatternAddStitching = new wxButton( m_panelPagePattern, ID_STITCHING, _("Add Stitching"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerPatternRight->Add( m_buttonOnPatternAddStitching, 0, wxALL|wxEXPAND, 5 );

	m_buttonOnPatternAddPadding = new wxButton( m_panelPagePattern, ID_PADDING, _("Add Padding"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerPatternRight->Add( m_buttonOnPatternAddPadding, 0, wxALL|wxEXPAND, 5 );

	m_buttonOnPatternAddCoordinateSystem = new wxButton( m_panelPagePattern, ID_COORDINATESYSTEM, _("Add Coordinate System"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerPatternRight->Add( m_buttonOnPatternAddCoordinateSystem, 0, wxALL|wxEXPAND, 5 );

	m_buttonOnPatternAddLaces = new wxButton( m_panelPagePattern, ID_LACES, _("Add Laces"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerPatternRight->Add( m_buttonOnPatternAddLaces, 0, wxALL|wxEXPAND, 5 );

	m_buttonOnPatternAddAccessory = new wxButton( m_panelPagePattern, ID_ACCESSORY, _("Add Accessory"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerPatternRight->Add( m_buttonOnPatternAddAccessory, 0, wxALL|wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizerPatternElementProperties;
	sbSizerPatternElementProperties = new wxStaticBoxSizer( new wxStaticBox( m_panelPagePattern, wxID_ANY, _("Properties") ), wxVERTICAL );

	m_propertyGridPatternElement = new wxPropertyGrid(sbSizerPatternElementProperties->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE|wxPG_HIDE_CATEGORIES|wxPG_HIDE_MARGIN|wxPG_SPLITTER_AUTO_CENTER|wxTAB_TRAVERSAL);
	m_propertyGridItemName = m_propertyGridPatternElement->Append( new wxStringProperty( _("Name"), _("Name") ) );
	m_propertyGridItemVisible = m_propertyGridPatternElement->Append( new wxBoolProperty( _("Visible"), _("Visible") ) );
	m_propertyGridItemFabric = m_propertyGridPatternElement->Append( new wxMultiChoiceProperty( _("Fabric"), _("Fabric") ) );
	sbSizerPatternElementProperties->Add( m_propertyGridPatternElement, 0, wxEXPAND|wxALL, 5 );


	bSizerPatternRight->Add( sbSizerPatternElementProperties, 0, wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizerFabric;
	sbSizerFabric = new wxStaticBoxSizer( new wxStaticBox( m_panelPagePattern, wxID_ANY, _("Fabrics") ), wxVERTICAL );

	m_listBoxFabric = new wxListBox( sbSizerFabric->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	m_listBoxFabric->Append( _("Leather - black") );
	m_listBoxFabric->Append( _("Leather - red") );
	m_listBoxFabric->Append( _("Leather - white") );
	m_listBoxFabric->Append( _("Patent - black") );
	m_listBoxFabric->Append( _("Patent - white") );
	m_listBoxFabric->Append( _("Cotton - black") );
	m_listBoxFabric->Append( _("Cotton - yellow") );
	m_listBoxFabric->Append( _("Cotton - red") );
	sbSizerFabric->Add( m_listBoxFabric, 2, wxALL|wxEXPAND, 5 );


	bSizerPatternRight->Add( sbSizerFabric, 1, wxEXPAND, 5 );


	bSizerPattern->Add( bSizerPatternRight, 0, wxEXPAND, 5 );


	m_panelPagePattern->SetSizer( bSizerPattern );
	m_panelPagePattern->Layout();
	bSizerPattern->Fit( m_panelPagePattern );
	m_notebook->AddPage( m_panelPagePattern, _("Pattern"), false );

	bSizerConfig->Add( m_notebook, 1, wxEXPAND | wxALL, 5 );


	m_panelConfig->SetSizer( bSizerConfig );
	m_panelConfig->Layout();
	bSizerConfig->Fit( m_panelConfig );
	m_panelCanvas = new wxPanel( m_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerCanvas;
	bSizerCanvas = new wxBoxSizer( wxVERTICAL );

	m_notebookCanvas = new wxNotebook( m_panelCanvas, ID_MAINVIEW, wxDefaultPosition, wxDefaultSize, 0 );
	m_panelCanvas3D = new wxPanel( m_notebookCanvas, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerCanvas3D;
	bSizerCanvas3D = new wxBoxSizer( wxVERTICAL );

	m_canvas3D = new Canvas3D( m_panelCanvas3D, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizerCanvas3D->Add( m_canvas3D, 1, wxALL|wxEXPAND, 5 );


	m_panelCanvas3D->SetSizer( bSizerCanvas3D );
	m_panelCanvas3D->Layout();
	bSizerCanvas3D->Fit( m_panelCanvas3D );
	m_notebookCanvas->AddPage( m_panelCanvas3D, _("3D"), false );
	m_panelCanvasInsole = new wxPanel( m_notebookCanvas, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerCanvasInsole;
	bSizerCanvasInsole = new wxBoxSizer( wxVERTICAL );

	m_canvasInsole = new CanvasInsole( m_panelCanvasInsole, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizerCanvasInsole->Add( m_canvasInsole, 1, wxALL|wxEXPAND, 5 );


	m_panelCanvasInsole->SetSizer( bSizerCanvasInsole );
	m_panelCanvasInsole->Layout();
	bSizerCanvasInsole->Fit( m_panelCanvasInsole );
	m_notebookCanvas->AddPage( m_panelCanvasInsole, _("Insole"), false );
	m_panelCanvasPattern = new wxPanel( m_notebookCanvas, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerCanvasPattern;
	bSizerCanvasPattern = new wxBoxSizer( wxVERTICAL );

	m_canvasPattern = new CanvasPattern( m_panelCanvasPattern, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizerCanvasPattern->Add( m_canvasPattern, 1, wxALL|wxEXPAND, 5 );


	m_panelCanvasPattern->SetSizer( bSizerCanvasPattern );
	m_panelCanvasPattern->Layout();
	bSizerCanvasPattern->Fit( m_panelCanvasPattern );
	m_notebookCanvas->AddPage( m_panelCanvasPattern, _("Pattern"), false );
	m_panelFlattening = new wxPanel( m_notebookCanvas, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerCanvasFlattening;
	bSizerCanvasFlattening = new wxBoxSizer( wxVERTICAL );

	m_canvasFlattening = new CanvasFlattening( m_panelFlattening, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_canvasFlattening->Hide();

	bSizerCanvasFlattening->Add( m_canvasFlattening, 1, wxALL|wxEXPAND, 5 );

	m_canvasGraph = new CanvasGraph( m_panelFlattening, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizerCanvasFlattening->Add( m_canvasGraph, 1, wxALL|wxEXPAND, 5 );


	m_panelFlattening->SetSizer( bSizerCanvasFlattening );
	m_panelFlattening->Layout();
	bSizerCanvasFlattening->Fit( m_panelFlattening );
	m_notebookCanvas->AddPage( m_panelFlattening, _("Flattening"), true );
	m_panelCanvasTestStitch = new wxPanel( m_notebookCanvas, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerCanvasTestStitch;
	bSizerCanvasTestStitch = new wxBoxSizer( wxVERTICAL );

	m_canvasTestStitch = new CanvasTestStitch( m_panelCanvasTestStitch, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizerCanvasTestStitch->Add( m_canvasTestStitch, 1, wxALL|wxEXPAND, 5 );


	m_panelCanvasTestStitch->SetSizer( bSizerCanvasTestStitch );
	m_panelCanvasTestStitch->Layout();
	bSizerCanvasTestStitch->Fit( m_panelCanvasTestStitch );
	m_notebookCanvas->AddPage( m_panelCanvasTestStitch, _("Test Stitch"), false );
	m_panelCanvasAnalysis = new wxPanel( m_notebookCanvas, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerCanvasAnalysis;
	bSizerCanvasAnalysis = new wxBoxSizer( wxHORIZONTAL );

	m_canvasSupport = new CanvasSupport( m_panelCanvasAnalysis, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizerCanvasAnalysis->Add( m_canvasSupport, 1, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizerAnalysisSupport;
	bSizerAnalysisSupport = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* sbSizerCycle;
	sbSizerCycle = new wxStaticBoxSizer( new wxStaticBox( m_panelCanvasAnalysis, wxID_ANY, _("Walkcycle") ), wxVERTICAL );

	m_canvasCycle = new CanvasWalkcycle( sbSizerCycle->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	sbSizerCycle->Add( m_canvasCycle, 1, wxALL|wxEXPAND, 5 );

	wxWrapSizer* wSizer4;
	wSizer4 = new wxWrapSizer( wxHORIZONTAL, 0 );

	m_checkBoxLockAnkle = new wxCheckBox( sbSizerCycle->GetStaticBox(), ID_ANKLELOCK, _("Lock Ankle"), wxDefaultPosition, wxDefaultSize, 0 );
	wSizer4->Add( m_checkBoxLockAnkle, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_sliderSpeed = new wxSlider( sbSizerCycle->GetStaticBox(), ID_SPEED, 833, 0, 8000, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	wSizer4->Add( m_sliderSpeed, 1, wxALL|wxEXPAND, 5 );

	m_textCtrlSpeed = new ExtendedTextCtrl( sbSizerCycle->GetStaticBox(), ID_SPEED, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	wSizer4->Add( m_textCtrlSpeed, 0, wxALL, 5 );


	sbSizerCycle->Add( wSizer4, 0, wxEXPAND, 5 );


	bSizerAnalysisSupport->Add( sbSizerCycle, 1, wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizerPlot;
	sbSizerPlot = new wxStaticBoxSizer( new wxStaticBox( m_panelCanvasAnalysis, wxID_ANY, _("Forces, Moments") ), wxVERTICAL );

	wxString m_choiceDisplayChoices[] = { _("Kneeline"), _("Force"), _("Moment"), _("Crossforce") };
	int m_choiceDisplayNChoices = sizeof( m_choiceDisplayChoices ) / sizeof( wxString );
	m_choiceDisplay = new wxChoice( sbSizerPlot->GetStaticBox(), ID_DISPLAY, wxDefaultPosition, wxDefaultSize, m_choiceDisplayNChoices, m_choiceDisplayChoices, 0 );
	m_choiceDisplay->SetSelection( 0 );
	sbSizerPlot->Add( m_choiceDisplay, 0, wxALL|wxEXPAND, 5 );


	bSizerAnalysisSupport->Add( sbSizerPlot, 1, wxEXPAND, 5 );


	bSizerCanvasAnalysis->Add( bSizerAnalysisSupport, 1, wxEXPAND, 5 );


	m_panelCanvasAnalysis->SetSizer( bSizerCanvasAnalysis );
	m_panelCanvasAnalysis->Layout();
	bSizerCanvasAnalysis->Fit( m_panelCanvasAnalysis );
	m_notebookCanvas->AddPage( m_panelCanvasAnalysis, _("Analysis"), false );

	bSizerCanvas->Add( m_notebookCanvas, 1, wxEXPAND | wxALL, 5 );


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
	m_menuFoot->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnMeasurementsQuickSetup ), this, m_menuItemQuickSetup->GetId());
	m_menuFootParameters->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnModelChanged ), this, m_menuItemFromMeasurements->GetId());
	m_menuFootParameters->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnModelChanged ), this, m_menuItemFromScan->GetId());
	m_menuMeasurementsCopy->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnMeasurementsCopy ), this, m_menuItemMeasurementsCopyActiveToInactive->GetId());
	m_menuMeasurementsCopy->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnMeasurementsCopy ), this, m_menuItemMeasurementsCopyRightToLeft->GetId());
	m_menuMeasurementsCopy->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnMeasurementsCopy ), this, m_menuItemMeasurementsCopyLeftToRight->GetId());
	m_menuLastModel->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnModelChanged ), this, m_menuItemConstructedLast->GetId());
	m_menuLastModel->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnModelChanged ), this, m_menuItemBoneBasedModel->GetId());
	m_menuLastModel->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnModelChanged ), this, m_menuItemUseLastBasedModel->GetId());
	m_menuLast->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnObjectLoad ), this, m_menuItemLastLoadSTL->GetId());
	m_menuBoneModel->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnObjectEdit ), this, m_menuItemEditBoneModel->GetId());
	m_menuBoneModel->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnObjectLoad ), this, m_menuItemLoadBoneModel->GetId());
	m_menuConstruction->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnConstructionChanged ), this, m_menuItemTestGenerator->GetId());
	m_menuConstruction->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnConstructionChanged ), this, m_menuItemConstructionClassic->GetId());
	m_menuConstruction->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnConstructionChanged ), this, m_menuItemConstructionCemented->GetId());
	m_menuConstruction->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnConstructionChanged ), this, m_menuItemConstructionMolded->GetId());
	m_menuConstruction->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnConstructionChanged ), this, m_menuItemConstructionDutch->GetId());
	m_menuConstruction->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnConstructionChanged ), this, m_menuItemConstructionGeta->GetId());
	m_menuPattern->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnObjectLoad ), this, m_menuItemLoadPattern->GetId());
	m_menuPattern->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnObjectSave ), this, m_menuItemSavePattern->GetId());
	m_menuPattern->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnViewChanged ), this, m_menuItemShowFlattening->GetId());
	m_menuFabric->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnObjectLoad ), this, m_menuItemLoadFabricsLibrary->GetId());
	m_menuGeometry->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnObjectSave ), this, m_menuItemSaveLast->GetId());
	m_menuGeometry->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnObjectSave ), this, m_menuItemSaveInsole->GetId());
	m_menuGeometry->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnObjectSave ), this, m_menuItemSaveHeel->GetId());
	m_menuGeometry->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnObjectSave ), this, m_menuItemCutaway->GetId());
	m_menuGeometry->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnObjectSave ), this, m_menuItemPackZip->GetId());
	m_menuView->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnToggleStereo3D ), this, m_menuItemStereo3D->GetId());
	m_menuView->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnViewChanged ), this, m_menuItemShowLeft->GetId());
	m_menuView->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnViewChanged ), this, m_menuItemShowRight->GetId());
	m_menuView->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnViewChanged ), this, m_menuItemShowBones->GetId());
	m_menuView->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnViewChanged ), this, m_menuItemShowSkin->GetId());
	m_menuView->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnViewChanged ), this, m_menuItemShowLeg->GetId());
	m_menuView->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnViewChanged ), this, m_menuItemShowLast->GetId());
	m_menuView->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnViewChanged ), this, m_menuItemShowInsole->GetId());
	m_menuView->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnViewChanged ), this, m_menuIItemShowHeel->GetId());
	m_menuView->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnViewChanged ), this, m_menuItemShowUpper->GetId());
	m_menuView->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnViewChanged ), this, m_menuItemShowCutaway->GetId());
	m_menuView->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnViewChanged ), this, m_menuItemShowFloor->GetId());
	m_menuView->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnViewChanged ), this, m_menuItemShowCoordinateSystem->GetId());
	m_menuView->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnViewChanged ), this, m_menuItemShowBackground->GetId());
	m_menuView->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnBackgroundImagesSetup ), this, m_menuItemSetupBackground->GetId());
	m_menuHelp->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameMain::OnCalculator ), this, m_menuItemCalculator->GetId());
	m_panelPageFoot->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_radioBtnEditLeft->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( GUIFrameMain::OnRadioButton ), NULL, this );
	m_radioBtnEditBoth->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( GUIFrameMain::OnRadioButton ), NULL, this );
	m_radioBtnEditRight->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( GUIFrameMain::OnRadioButton ), NULL, this );
	m_choicebookMeasurementSource->Connect( wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGED, wxChoicebookEventHandler( GUIFrameMain::OnPageChanged ), NULL, this );
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
	m_textCtrlLegLengthDifference->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlLegLengthDifference->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlLegLengthDifference->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlLegLengthDifference->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_buttonQuickSetup->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnMeasurementsQuickSetup ), NULL, this );
	m_bitmapFoot->Connect( wxEVT_SIZE, wxSizeEventHandler( GUIFrameMain::OnSize ), NULL, this );
	m_filePickerScan->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( GUIFrameMain::OnFileChanged ), NULL, this );
	m_radioBtnEditLegLeft->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( GUIFrameMain::OnRadioButton ), NULL, this );
	m_radioBtnEditLegBoth->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( GUIFrameMain::OnRadioButton ), NULL, this );
	m_radioBtnEditLegRight->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( GUIFrameMain::OnRadioButton ), NULL, this );
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
	m_choicebookLastConstructionType->Connect( wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGED, wxChoicebookEventHandler( GUIFrameMain::OnPageChanged ), NULL, this );
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
	m_textCtrlTipSharpness->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlTipSharpness->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlTipSharpness->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlTipSharpness->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlAngleMixing->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlAngleMixing->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlAngleMixing->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlAngleMixing->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlExtraLength->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlExtraLength->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlExtraLength->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlExtraLength->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlFootCompression->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlFootCompression->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlFootCompression->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlFootCompression->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_filePickerBoneModel->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( GUIFrameMain::OnFileChanged ), NULL, this );
	m_buttonEditBoneModel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnObjectEdit ), NULL, this );
	m_filePickerLast->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( GUIFrameMain::OnFileChanged ), NULL, this );
	m_checkBoxLastModify->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GUIFrameMain::OnCheckBox ), NULL, this );
	m_checkBoxLastReorient->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GUIFrameMain::OnCheckBox ), NULL, this );
	m_choicebookHeelConstructionType->Connect( wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGED, wxChoicebookEventHandler( GUIFrameMain::OnPageChanged ), NULL, this );
	m_filePickerHeel->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( GUIFrameMain::OnFileChanged ), NULL, this );
	m_checkBoxHeelReorient->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GUIFrameMain::OnCheckBox ), NULL, this );
	m_choicePresetShoeType->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GUIFrameMain::OnChoice ), NULL, this );
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
	m_choicePresetShoeStyle->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GUIFrameMain::OnChoice ), NULL, this );
	m_textCtrlUpperLevel->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlUpperLevel->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlUpperLevel->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlUpperLevel->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_choicebookConstruction->Connect( wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGED, wxChoicebookEventHandler( GUIFrameMain::OnPageChanged ), NULL, this );
	m_textCtrlWeltSize->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlWeltSize->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlWeltSize->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlWeltSize->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlThickness->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlThickness->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlThickness->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlThickness->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_buttonAddBridge->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnButtonAdd ), NULL, this );
	m_buttonDeleteBridge->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnButtonDelete ), NULL, this );
	m_dataViewListCtrlBridges->Connect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( GUIFrameMain::OnListCtrlOnSelectionChanged ), NULL, this );
	m_textCtrlSupportHeelRadius->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlSupportHeelRadius->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlSupportHeelRadius->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlSupportHeelRadius->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlSupportHeelOffset->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlSupportHeelOffset->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlSupportHeelOffset->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlSupportHeelOffset->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlSupportToeRadius->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlSupportToeRadius->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlSupportToeRadius->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlSupportToeRadius->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlSupportToeOffset->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlSupportToeOffset->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlSupportToeOffset->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlSupportToeOffset->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_choicePresetHeel->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GUIFrameMain::OnChoice ), NULL, this );
	m_choicePresetHeelCode->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GUIFrameMain::OnChoice ), NULL, this );
	m_choicePresetHeelVariant->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GUIFrameMain::OnChoice ), NULL, this );
	m_editorCode->Connect( wxEVT_CHAR, wxKeyEventHandler( GUIFrameMain::OnChar ), NULL, this );
	m_dataViewListCtrlVariant->Connect( wxEVT_COMMAND_DATAVIEW_ITEM_EDITING_DONE, wxDataViewEventHandler( GUIFrameMain::OnDataViewListCtrlItemEditingDone ), NULL, this );
	m_treeListCtrlPattern->Connect( wxEVT_TREELIST_SELECTION_CHANGED, wxTreeListEventHandler( GUIFrameMain::OnPatternSelect ), NULL, this );
	m_buttonElementCopy->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnButtonCopy ), NULL, this );
	m_buttonElementDelete->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnButtonDelete ), NULL, this );
	m_buttonOnPatternAddLine->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnButtonAdd ), NULL, this );
	m_buttonOnPatternAddPatch->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnButtonAdd ), NULL, this );
	m_buttonOnPatternAddStitching->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnButtonAdd ), NULL, this );
	m_buttonOnPatternAddPadding->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnButtonAdd ), NULL, this );
	m_buttonOnPatternAddCoordinateSystem->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnButtonAdd ), NULL, this );
	m_buttonOnPatternAddLaces->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnButtonAdd ), NULL, this );
	m_buttonOnPatternAddAccessory->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnButtonAdd ), NULL, this );
	m_listBoxFabric->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( GUIFrameMain::OnPatternSelectFabric ), NULL, this );
	m_notebookCanvas->Connect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( GUIFrameMain::OnNotebookPageChanged ), NULL, this );
	m_canvas3D->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( GUIFrameMain::On3DSelect ), NULL, this );
	m_checkBoxLockAnkle->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GUIFrameMain::OnCheckBox ), NULL, this );
	m_sliderSpeed->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSpeed->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSpeed->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSpeed->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSpeed->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSpeed->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSpeed->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSpeed->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSpeed->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_textCtrlSpeed->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlSpeed->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlSpeed->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_choiceDisplay->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GUIFrameMain::OnChoice ), NULL, this );
}

GUIFrameMain::~GUIFrameMain()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GUIFrameMain::OnClose ) );
	m_panelPageFoot->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_radioBtnEditLeft->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( GUIFrameMain::OnRadioButton ), NULL, this );
	m_radioBtnEditBoth->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( GUIFrameMain::OnRadioButton ), NULL, this );
	m_radioBtnEditRight->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( GUIFrameMain::OnRadioButton ), NULL, this );
	m_choicebookMeasurementSource->Disconnect( wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGED, wxChoicebookEventHandler( GUIFrameMain::OnPageChanged ), NULL, this );
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
	m_textCtrlLegLengthDifference->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlLegLengthDifference->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlLegLengthDifference->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlLegLengthDifference->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_buttonQuickSetup->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnMeasurementsQuickSetup ), NULL, this );
	m_bitmapFoot->Disconnect( wxEVT_SIZE, wxSizeEventHandler( GUIFrameMain::OnSize ), NULL, this );
	m_filePickerScan->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( GUIFrameMain::OnFileChanged ), NULL, this );
	m_radioBtnEditLegLeft->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( GUIFrameMain::OnRadioButton ), NULL, this );
	m_radioBtnEditLegBoth->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( GUIFrameMain::OnRadioButton ), NULL, this );
	m_radioBtnEditLegRight->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( GUIFrameMain::OnRadioButton ), NULL, this );
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
	m_choicebookLastConstructionType->Disconnect( wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGED, wxChoicebookEventHandler( GUIFrameMain::OnPageChanged ), NULL, this );
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
	m_textCtrlTipSharpness->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlTipSharpness->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlTipSharpness->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlTipSharpness->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlAngleMixing->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlAngleMixing->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlAngleMixing->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlAngleMixing->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlExtraLength->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlExtraLength->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlExtraLength->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlExtraLength->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlFootCompression->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlFootCompression->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlFootCompression->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlFootCompression->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_filePickerBoneModel->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( GUIFrameMain::OnFileChanged ), NULL, this );
	m_buttonEditBoneModel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnObjectEdit ), NULL, this );
	m_filePickerLast->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( GUIFrameMain::OnFileChanged ), NULL, this );
	m_checkBoxLastModify->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GUIFrameMain::OnCheckBox ), NULL, this );
	m_checkBoxLastReorient->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GUIFrameMain::OnCheckBox ), NULL, this );
	m_choicebookHeelConstructionType->Disconnect( wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGED, wxChoicebookEventHandler( GUIFrameMain::OnPageChanged ), NULL, this );
	m_filePickerHeel->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( GUIFrameMain::OnFileChanged ), NULL, this );
	m_checkBoxHeelReorient->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GUIFrameMain::OnCheckBox ), NULL, this );
	m_choicePresetShoeType->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GUIFrameMain::OnChoice ), NULL, this );
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
	m_choicePresetShoeStyle->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GUIFrameMain::OnChoice ), NULL, this );
	m_textCtrlUpperLevel->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlUpperLevel->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlUpperLevel->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlUpperLevel->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_choicebookConstruction->Disconnect( wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGED, wxChoicebookEventHandler( GUIFrameMain::OnPageChanged ), NULL, this );
	m_textCtrlWeltSize->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlWeltSize->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlWeltSize->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlWeltSize->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlThickness->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlThickness->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlThickness->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlThickness->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_buttonAddBridge->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnButtonAdd ), NULL, this );
	m_buttonDeleteBridge->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnButtonDelete ), NULL, this );
	m_dataViewListCtrlBridges->Disconnect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( GUIFrameMain::OnListCtrlOnSelectionChanged ), NULL, this );
	m_textCtrlSupportHeelRadius->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlSupportHeelRadius->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlSupportHeelRadius->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlSupportHeelRadius->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlSupportHeelOffset->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlSupportHeelOffset->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlSupportHeelOffset->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlSupportHeelOffset->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlSupportToeRadius->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlSupportToeRadius->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlSupportToeRadius->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlSupportToeRadius->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_textCtrlSupportToeOffset->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlSupportToeOffset->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlSupportToeOffset->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( GUIFrameMain::OnSetFocus ), NULL, this );
	m_textCtrlSupportToeOffset->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_choicePresetHeel->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GUIFrameMain::OnChoice ), NULL, this );
	m_choicePresetHeelCode->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GUIFrameMain::OnChoice ), NULL, this );
	m_choicePresetHeelVariant->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GUIFrameMain::OnChoice ), NULL, this );
	m_editorCode->Disconnect( wxEVT_CHAR, wxKeyEventHandler( GUIFrameMain::OnChar ), NULL, this );
	m_dataViewListCtrlVariant->Disconnect( wxEVT_COMMAND_DATAVIEW_ITEM_EDITING_DONE, wxDataViewEventHandler( GUIFrameMain::OnDataViewListCtrlItemEditingDone ), NULL, this );
	m_treeListCtrlPattern->Disconnect( wxEVT_TREELIST_SELECTION_CHANGED, wxTreeListEventHandler( GUIFrameMain::OnPatternSelect ), NULL, this );
	m_buttonElementCopy->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnButtonCopy ), NULL, this );
	m_buttonElementDelete->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnButtonDelete ), NULL, this );
	m_buttonOnPatternAddLine->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnButtonAdd ), NULL, this );
	m_buttonOnPatternAddPatch->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnButtonAdd ), NULL, this );
	m_buttonOnPatternAddStitching->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnButtonAdd ), NULL, this );
	m_buttonOnPatternAddPadding->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnButtonAdd ), NULL, this );
	m_buttonOnPatternAddCoordinateSystem->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnButtonAdd ), NULL, this );
	m_buttonOnPatternAddLaces->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnButtonAdd ), NULL, this );
	m_buttonOnPatternAddAccessory->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameMain::OnButtonAdd ), NULL, this );
	m_listBoxFabric->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( GUIFrameMain::OnPatternSelectFabric ), NULL, this );
	m_notebookCanvas->Disconnect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( GUIFrameMain::OnNotebookPageChanged ), NULL, this );
	m_canvas3D->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( GUIFrameMain::On3DSelect ), NULL, this );
	m_checkBoxLockAnkle->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GUIFrameMain::OnCheckBox ), NULL, this );
	m_sliderSpeed->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSpeed->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSpeed->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSpeed->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSpeed->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSpeed->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSpeed->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSpeed->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_sliderSpeed->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( GUIFrameMain::OnScroll ), NULL, this );
	m_textCtrlSpeed->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameMain::OnKillFocus ), NULL, this );
	m_textCtrlSpeed->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( GUIFrameMain::OnMouseWheel ), NULL, this );
	m_textCtrlSpeed->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameMain::OnTextEnter ), NULL, this );
	m_choiceDisplay->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GUIFrameMain::OnChoice ), NULL, this );

}

GUIFrameBoneModel::GUIFrameBoneModel( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

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

	wxBoxSizer* bSizer;
	bSizer = new wxBoxSizer( wxVERTICAL );

	m_notebook = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_MULTILINE );
	m_panelBone = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerLength;
	bSizerLength = new wxBoxSizer( wxVERTICAL );


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

	m_gridBoneLength = new wxGrid( m_panelBoneDiameter, ID_GRIDLENGTH, wxDefaultPosition, wxDefaultSize, 0 );

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
	bSizerDiameter->Add( m_gridBoneLength, 1, wxALL|wxEXPAND, 5 );


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

	this->Centre( wxBOTH );

	// Connect Events
	m_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameBoneModel::OnReset ), this, m_menuItemReset->GetId());
	m_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameBoneModel::OnLoad ), this, m_menuItemLoad->GetId());
	m_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameBoneModel::OnSave ), this, m_menuItemSave->GetId());
	m_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrameBoneModel::OnSaveAs ), this, m_menuItemSaveAs->GetId());
	m_gridBoneDiameter->Connect( wxEVT_GRID_CELL_CHANGED, wxGridEventHandler( GUIFrameBoneModel::OnCellChange ), NULL, this );
	m_gridBoneLength->Connect( wxEVT_GRID_CELL_CHANGED, wxGridEventHandler( GUIFrameBoneModel::OnCellChange ), NULL, this );
	m_gridSkinThickness->Connect( wxEVT_GRID_CELL_CHANGED, wxGridEventHandler( GUIFrameBoneModel::OnCellChange ), NULL, this );
}

GUIFrameBoneModel::~GUIFrameBoneModel()
{
	// Disconnect Events
	m_gridBoneDiameter->Disconnect( wxEVT_GRID_CELL_CHANGED, wxGridEventHandler( GUIFrameBoneModel::OnCellChange ), NULL, this );
	m_gridBoneLength->Disconnect( wxEVT_GRID_CELL_CHANGED, wxGridEventHandler( GUIFrameBoneModel::OnCellChange ), NULL, this );
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

	m_textCtrlShoeSize = new ExtendedTextCtrl( this, wxID_ANY, _("39"), wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
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
	m_dataViewListColumnFormula = m_dataViewListCtrl->AppendTextColumn( _("Formula"), wxDATAVIEW_CELL_INERT, 400, static_cast<wxAlignment>(wxALIGN_LEFT), wxDATAVIEW_COL_RESIZABLE );
	m_dataViewListColumnValue = m_dataViewListCtrl->AppendTextColumn( _("Value"), wxDATAVIEW_CELL_INERT, -1, static_cast<wxAlignment>(wxALIGN_LEFT), wxDATAVIEW_COL_RESIZABLE );
	bSizerMain->Add( m_dataViewListCtrl, 1, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizerVariableName;
	bSizerVariableName = new wxBoxSizer( wxHORIZONTAL );

	m_staticTextVariableName = new wxStaticText( this, wxID_ANY, _("<>"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextVariableName->Wrap( -1 );
	bSizerVariableName->Add( m_staticTextVariableName, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	bSizerMain->Add( bSizerVariableName, 0, wxEXPAND, 5 );

	m_textCtrlFormula = new ExtendedTextCtrl( this, ID_FORMULA, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrlFormula->SetToolTip( _("Enter formula or value. \nAllowed operations are: +, -, *, /, ^, cos, sin, tan, acos, asin, atan, exp, log, sqrt, cbrt, ceil, floor, round, abs") );

	bSizerMain->Add( m_textCtrlFormula, 0, wxALL|wxEXPAND, 5 );

	m_textCtrlResult = new ExtendedTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	m_textCtrlResult->SetToolTip( _("Result") );

	bSizerMain->Add( m_textCtrlResult, 0, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizerButtons;
	bSizerButtons = new wxBoxSizer( wxHORIZONTAL );

	m_buttonClear = new wxButton( this, ID_FORMULA, _("Clear formula"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerButtons->Add( m_buttonClear, 0, wxALL, 5 );

	m_buttonCopyResultFormula = new wxButton( this, ID_FORMULA, _("Copy result to formula"), wxDefaultPosition, wxDefaultSize, 0 );
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
	m_textCtrlFormula->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIDialogFormulaEditor::OnTextEnter ), NULL, this );
	m_buttonClear->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIDialogFormulaEditor::OnButtonClear ), NULL, this );
	m_buttonCopyResultFormula->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIDialogFormulaEditor::OnButtonCopy ), NULL, this );
}

GUIDialogFormulaEditor::~GUIDialogFormulaEditor()
{
	// Disconnect Events
	m_dataViewListCtrl->Disconnect( wxEVT_COMMAND_DATAVIEW_ITEM_ACTIVATED, wxDataViewEventHandler( GUIDialogFormulaEditor::OnItemActivated ), NULL, this );
	m_textCtrlFormula->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIDialogFormulaEditor::OnTextEnter ), NULL, this );
	m_buttonClear->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIDialogFormulaEditor::OnButtonClear ), NULL, this );
	m_buttonCopyResultFormula->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIDialogFormulaEditor::OnButtonCopy ), NULL, this );

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

	m_buttonAdd = new wxButton( this, ID_SELECTION, _("Add/Update"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerButtons->Add( m_buttonAdd, 1, wxALL, 5 );

	m_buttonRemove = new wxButton( this, ID_SELECTION, _("Remove"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerButtons->Add( m_buttonRemove, 1, wxALL, 5 );


	bSizerLeftColumn->Add( bSizerButtons, 0, wxEXPAND, 5 );


	bSizerMain->Add( bSizerLeftColumn, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizerRightColumn;
	bSizerRightColumn = new wxBoxSizer( wxVERTICAL );

	m_staticTextFilename = new wxStaticText( this, wxID_ANY, _("Filename:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextFilename->Wrap( -1 );
	bSizerRightColumn->Add( m_staticTextFilename, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	m_filePicker = new wxFilePickerCtrl( this, ID_IMAGE, wxEmptyString, _("Select a file"), _("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	bSizerRightColumn->Add( m_filePicker, 0, wxALL|wxEXPAND, 5 );

	m_staticTextDirection = new wxStaticText( this, wxID_ANY, _("Visible from"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDirection->Wrap( -1 );
	bSizerRightColumn->Add( m_staticTextDirection, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	wxGridSizer* gSizerDirection;
	gSizerDirection = new wxGridSizer( 0, 2, 0, 0 );

	m_checkBoxFront = new wxCheckBox( this, ID_FRONT, _("Front"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizerDirection->Add( m_checkBoxFront, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	m_checkBoxBack = new wxCheckBox( this, ID_BACK, _("Back"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizerDirection->Add( m_checkBoxBack, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	m_checkBoxLeft = new wxCheckBox( this, ID_LEFT, _("Left"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizerDirection->Add( m_checkBoxLeft, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	m_checkBoxRight = new wxCheckBox( this, ID_RIGHT, _("Right"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizerDirection->Add( m_checkBoxRight, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	m_checkBoxTop = new wxCheckBox( this, ID_TOP, _("Top"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizerDirection->Add( m_checkBoxTop, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	m_checkBoxBottom = new wxCheckBox( this, ID_BOTTOM, _("Bottom"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizerDirection->Add( m_checkBoxBottom, 0, wxTOP|wxRIGHT|wxLEFT, 5 );


	bSizerRightColumn->Add( gSizerDirection, 0, wxEXPAND|wxBOTTOM, 5 );

	m_staticText46 = new wxStaticText( this, wxID_ANY, _("Scale:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText46->Wrap( -1 );
	bSizerRightColumn->Add( m_staticText46, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	wxBoxSizer* bSizerScale;
	bSizerScale = new wxBoxSizer( wxHORIZONTAL );

	m_textCtrlScale = new ExtendedTextCtrl( this, ID_SCALE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizerScale->Add( m_textCtrlScale, 1, wxTOP|wxBOTTOM|wxLEFT, 5 );

	m_spinBtnScale = new wxSpinButton( this, ID_SCALE, wxDefaultPosition, wxDefaultSize, 0 );
	bSizerScale->Add( m_spinBtnScale, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );


	bSizerRightColumn->Add( bSizerScale, 0, wxEXPAND, 5 );

	m_staticTextOffsHor = new wxStaticText( this, wxID_ANY, _("Horizontal offset:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextOffsHor->Wrap( -1 );
	bSizerRightColumn->Add( m_staticTextOffsHor, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	wxBoxSizer* bSizerHorizontalOffset;
	bSizerHorizontalOffset = new wxBoxSizer( wxHORIZONTAL );

	m_textCtrlOffsHor = new ExtendedTextCtrl( this, ID_OFFSETHORIZONTAL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizerHorizontalOffset->Add( m_textCtrlOffsHor, 1, wxTOP|wxBOTTOM|wxLEFT, 5 );

	m_spinBtnOffsHor = new wxSpinButton( this, ID_OFFSETHORIZONTAL, wxDefaultPosition, wxDefaultSize, 0 );
	bSizerHorizontalOffset->Add( m_spinBtnOffsHor, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );


	bSizerRightColumn->Add( bSizerHorizontalOffset, 0, wxEXPAND, 5 );

	m_staticTextOffsVert = new wxStaticText( this, wxID_ANY, _("Vertical Offset:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextOffsVert->Wrap( -1 );
	bSizerRightColumn->Add( m_staticTextOffsVert, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	wxBoxSizer* bSizerVerticalOffset;
	bSizerVerticalOffset = new wxBoxSizer( wxHORIZONTAL );

	m_textCtrlOffsVert = new ExtendedTextCtrl( this, ID_OFFSETVERTICAL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizerVerticalOffset->Add( m_textCtrlOffsVert, 1, wxTOP|wxBOTTOM|wxLEFT, 5 );

	m_spinBtnOffsVert = new wxSpinButton( this, ID_OFFSETVERTICAL, wxDefaultPosition, wxDefaultSize, 0 );
	bSizerVerticalOffset->Add( m_spinBtnOffsVert, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );


	bSizerRightColumn->Add( bSizerVerticalOffset, 0, wxEXPAND, 5 );

	m_staticTextRotation = new wxStaticText( this, wxID_ANY, _("Rotation:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextRotation->Wrap( -1 );
	bSizerRightColumn->Add( m_staticTextRotation, 0, wxTOP|wxRIGHT|wxLEFT, 5 );

	m_sliderRotation = new wxSlider( this, ID_ROTATION, 0, -180, 180, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_HORIZONTAL|wxSL_LABELS );
	bSizerRightColumn->Add( m_sliderRotation, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );


	bSizerRightColumn->Add( 0, 0, 1, wxEXPAND, 5 );

	m_buttonClose = new wxButton( this, wxID_CLOSE, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerRightColumn->Add( m_buttonClose, 0, wxALL|wxEXPAND, 5 );


	bSizerMain->Add( bSizerRightColumn, 1, wxEXPAND, 5 );


	this->SetSizer( bSizerMain );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_buttonAdd->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameSetupBackgroundImages::OnButtonAdd ), NULL, this );
	m_buttonRemove->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameSetupBackgroundImages::OnButtonRemove ), NULL, this );
	m_checkBoxFront->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GUIFrameSetupBackgroundImages::OnCheckBox ), NULL, this );
	m_checkBoxBack->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GUIFrameSetupBackgroundImages::OnCheckBox ), NULL, this );
	m_checkBoxLeft->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GUIFrameSetupBackgroundImages::OnCheckBox ), NULL, this );
	m_checkBoxRight->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GUIFrameSetupBackgroundImages::OnCheckBox ), NULL, this );
	m_checkBoxTop->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GUIFrameSetupBackgroundImages::OnCheckBox ), NULL, this );
	m_checkBoxBottom->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GUIFrameSetupBackgroundImages::OnCheckBox ), NULL, this );
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
	m_buttonAdd->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameSetupBackgroundImages::OnButtonAdd ), NULL, this );
	m_buttonRemove->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameSetupBackgroundImages::OnButtonRemove ), NULL, this );
	m_checkBoxFront->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GUIFrameSetupBackgroundImages::OnCheckBox ), NULL, this );
	m_checkBoxBack->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GUIFrameSetupBackgroundImages::OnCheckBox ), NULL, this );
	m_checkBoxLeft->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GUIFrameSetupBackgroundImages::OnCheckBox ), NULL, this );
	m_checkBoxRight->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GUIFrameSetupBackgroundImages::OnCheckBox ), NULL, this );
	m_checkBoxTop->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GUIFrameSetupBackgroundImages::OnCheckBox ), NULL, this );
	m_checkBoxBottom->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GUIFrameSetupBackgroundImages::OnCheckBox ), NULL, this );
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

	m_staticTextMeasurements = new wxStaticText( this, wxID_ANY, _("Directory for measurements:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextMeasurements->Wrap( -1 );
	bSizer->Add( m_staticTextMeasurements, 0, wxALL, 5 );

	m_dirPickerMeasurementsDirectory = new wxDirPickerCtrl( this, wxID_ANY, wxEmptyString, _("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE|wxDIRP_DIR_MUST_EXIST|wxDIRP_USE_TEXTCTRL );
	bSizer->Add( m_dirPickerMeasurementsDirectory, 0, wxALL|wxEXPAND, 5 );

	m_staticTextPresetDirectory = new wxStaticText( this, wxID_ANY, _("Directory for presets:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPresetDirectory->Wrap( -1 );
	bSizer->Add( m_staticTextPresetDirectory, 0, wxALL, 5 );

	m_dirPickerPresetDirectory = new wxDirPickerCtrl( this, wxID_ANY, wxEmptyString, _("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE|wxDIRP_DIR_MUST_EXIST|wxDIRP_USE_TEXTCTRL );
	bSizer->Add( m_dirPickerPresetDirectory, 0, wxALL|wxEXPAND, 5 );

	m_staticTextLastDirectory = new wxStaticText( this, wxID_ANY, _("Directory for lasts:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextLastDirectory->Wrap( -1 );
	bSizer->Add( m_staticTextLastDirectory, 0, wxALL, 5 );

	m_dirPickerLastDirectory = new wxDirPickerCtrl( this, wxID_ANY, wxEmptyString, _("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE|wxDIRP_DIR_MUST_EXIST|wxDIRP_USE_TEXTCTRL );
	bSizer->Add( m_dirPickerLastDirectory, 0, wxALL|wxEXPAND, 5 );

	m_staticTextFabricDirectory = new wxStaticText( this, wxID_ANY, _("Fabric library:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextFabricDirectory->Wrap( -1 );
	bSizer->Add( m_staticTextFabricDirectory, 0, wxALL, 5 );

	m_dirPickerFabricDirectory = new wxDirPickerCtrl( this, wxID_ANY, wxEmptyString, _("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE|wxDIRP_DIR_MUST_EXIST|wxDIRP_USE_TEXTCTRL );
	bSizer->Add( m_dirPickerFabricDirectory, 0, wxALL|wxEXPAND, 5 );

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
	m_dirPickerMeasurementsDirectory->Connect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( GUISetupPaths::OnDirChanged ), NULL, this );
	m_dirPickerPresetDirectory->Connect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( GUISetupPaths::OnDirChanged ), NULL, this );
	m_dirPickerLastDirectory->Connect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( GUISetupPaths::OnDirChanged ), NULL, this );
	m_dirPickerFabricDirectory->Connect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( GUISetupPaths::OnDirChanged ), NULL, this );
	m_dirPickerOutputDirectory->Connect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( GUISetupPaths::OnDirChanged ), NULL, this );
	m_buttonClose->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUISetupPaths::OnClose ), NULL, this );
}

GUISetupPaths::~GUISetupPaths()
{
	// Disconnect Events
	m_dirPickerMeasurementsDirectory->Disconnect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( GUISetupPaths::OnDirChanged ), NULL, this );
	m_dirPickerPresetDirectory->Disconnect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( GUISetupPaths::OnDirChanged ), NULL, this );
	m_dirPickerLastDirectory->Disconnect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( GUISetupPaths::OnDirChanged ), NULL, this );
	m_dirPickerFabricDirectory->Disconnect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( GUISetupPaths::OnDirChanged ), NULL, this );
	m_dirPickerOutputDirectory->Disconnect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( GUISetupPaths::OnDirChanged ), NULL, this );
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


	bSizer->Add( 0, 0, 1, wxEXPAND, 5 );

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

	m_buttonAdd = new wxButton( this, ID_POINT, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerPoints->Add( m_buttonAdd, 0, wxALL, 5 );

	m_buttonRemove = new wxButton( this, ID_POINT, _("Remove"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerPoints->Add( m_buttonRemove, 0, wxALL, 5 );


	bSizerAnisotropy->Add( bSizerPoints, 0, wxEXPAND, 5 );

	m_canvasAnisotropy = new CanvasAnisotropy(this);
	m_canvasAnisotropy->Hide();

	bSizerAnisotropy->Add( m_canvasAnisotropy, 0, wxALL, 5 );


	this->SetSizer( bSizerAnisotropy );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_buttonAdd->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIDialogAnisotropy::OnButtonAdd ), NULL, this );
	m_buttonRemove->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIDialogAnisotropy::OnButtonRemove ), NULL, this );
}

GUIDialogAnisotropy::~GUIDialogAnisotropy()
{
	// Disconnect Events
	m_buttonAdd->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIDialogAnisotropy::OnButtonAdd ), NULL, this );
	m_buttonRemove->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIDialogAnisotropy::OnButtonRemove ), NULL, this );

}

GUIFrameCalculator::GUIFrameCalculator( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizerSplitterVertical;
	bSizerSplitterVertical = new wxBoxSizer( wxVERTICAL );

	m_splitterVertical = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitterVertical->SetSashGravity( 1 );
	m_splitterVertical->Connect( wxEVT_IDLE, wxIdleEventHandler( GUIFrameCalculator::m_splitterVerticalOnIdle ), NULL, this );

	m_panelLeft = new wxPanel( m_splitterVertical, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerSplitterHorizontal;
	bSizerSplitterHorizontal = new wxBoxSizer( wxVERTICAL );

	m_splitterHorizontal = new wxSplitterWindow( m_panelLeft, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitterHorizontal->SetSashGravity( 0.1 );
	m_splitterHorizontal->Connect( wxEVT_IDLE, wxIdleEventHandler( GUIFrameCalculator::m_splitterHorizontalOnIdle ), NULL, this );

	m_panelTop = new wxPanel( m_splitterHorizontal, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerTop;
	bSizerTop = new wxBoxSizer( wxVERTICAL );

	m_textCtrlCode = new wxStyledTextCtrl( m_panelTop, ID_CODE, wxDefaultPosition, wxDefaultSize, 0, wxEmptyString );
	m_textCtrlCode->SetUseTabs( true );
	m_textCtrlCode->SetTabWidth( 4 );
	m_textCtrlCode->SetIndent( 4 );
	m_textCtrlCode->SetTabIndents( true );
	m_textCtrlCode->SetBackSpaceUnIndents( true );
	m_textCtrlCode->SetViewEOL( false );
	m_textCtrlCode->SetViewWhiteSpace( false );
	m_textCtrlCode->SetMarginWidth( 2, 0 );
	m_textCtrlCode->SetIndentationGuides( true );
	m_textCtrlCode->SetReadOnly( false );
	m_textCtrlCode->SetMarginType( 1, wxSTC_MARGIN_SYMBOL );
	m_textCtrlCode->SetMarginMask( 1, wxSTC_MASK_FOLDERS );
	m_textCtrlCode->SetMarginWidth( 1, 16);
	m_textCtrlCode->SetMarginSensitive( 1, true );
	m_textCtrlCode->SetProperty( wxT("fold"), wxT("1") );
	m_textCtrlCode->SetFoldFlags( wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED | wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED );
	m_textCtrlCode->SetMarginType( 0, wxSTC_MARGIN_NUMBER );
	m_textCtrlCode->SetMarginWidth( 0, m_textCtrlCode->TextWidth( wxSTC_STYLE_LINENUMBER, wxT("_99999") ) );
	m_textCtrlCode->MarkerDefine( wxSTC_MARKNUM_FOLDER, wxSTC_MARK_BOXPLUS );
	m_textCtrlCode->MarkerSetBackground( wxSTC_MARKNUM_FOLDER, wxColour( wxT("BLACK") ) );
	m_textCtrlCode->MarkerSetForeground( wxSTC_MARKNUM_FOLDER, wxColour( wxT("WHITE") ) );
	m_textCtrlCode->MarkerDefine( wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_BOXMINUS );
	m_textCtrlCode->MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPEN, wxColour( wxT("BLACK") ) );
	m_textCtrlCode->MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPEN, wxColour( wxT("WHITE") ) );
	m_textCtrlCode->MarkerDefine( wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_EMPTY );
	m_textCtrlCode->MarkerDefine( wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_BOXPLUS );
	m_textCtrlCode->MarkerSetBackground( wxSTC_MARKNUM_FOLDEREND, wxColour( wxT("BLACK") ) );
	m_textCtrlCode->MarkerSetForeground( wxSTC_MARKNUM_FOLDEREND, wxColour( wxT("WHITE") ) );
	m_textCtrlCode->MarkerDefine( wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_BOXMINUS );
	m_textCtrlCode->MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPENMID, wxColour( wxT("BLACK") ) );
	m_textCtrlCode->MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPENMID, wxColour( wxT("WHITE") ) );
	m_textCtrlCode->MarkerDefine( wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY );
	m_textCtrlCode->MarkerDefine( wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_EMPTY );
	m_textCtrlCode->SetSelBackground( true, wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );
	m_textCtrlCode->SetSelForeground( true, wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHTTEXT ) );
	bSizerTop->Add( m_textCtrlCode, 1, wxEXPAND | wxALL, 5 );


	m_panelTop->SetSizer( bSizerTop );
	m_panelTop->Layout();
	bSizerTop->Fit( m_panelTop );
	m_panelBottom = new wxPanel( m_splitterHorizontal, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerBottom;
	bSizerBottom = new wxBoxSizer( wxVERTICAL );

	m_notebook = new wxNotebook( m_panelBottom, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panelX = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerX;
	bSizerX = new wxBoxSizer( wxVERTICAL );

	m_canvasGraph = new CanvasGraph( m_panelX, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizerX->Add( m_canvasGraph, 1, wxALL|wxEXPAND, 5 );

	wxWrapSizer* wSizerX;
	wSizerX = new wxWrapSizer( wxHORIZONTAL, 0 );


	wSizerX->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticTextXMin = new wxStaticText( m_panelX, wxID_ANY, _("x min:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextXMin->Wrap( -1 );
	wSizerX->Add( m_staticTextXMin, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrlXMin = new ExtendedTextCtrl( m_panelX, ID_XMIN, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTER|wxTE_PROCESS_ENTER );
	wSizerX->Add( m_textCtrlXMin, 0, wxALL, 5 );

	m_staticTextXMax = new wxStaticText( m_panelX, wxID_ANY, _("x max:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextXMax->Wrap( -1 );
	wSizerX->Add( m_staticTextXMax, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrlXMax = new ExtendedTextCtrl( m_panelX, ID_XMAX, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTER|wxTE_PROCESS_ENTER );
	wSizerX->Add( m_textCtrlXMax, 0, wxALL, 5 );


	wSizerX->Add( 0, 0, 1, wxEXPAND, 5 );


	bSizerX->Add( wSizerX, 0, wxEXPAND, 5 );


	m_panelX->SetSizer( bSizerX );
	m_panelX->Layout();
	bSizerX->Fit( m_panelX );
	m_notebook->AddPage( m_panelX, _("y = f(x)"), true );
	m_panelXY = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerXY;
	bSizerXY = new wxBoxSizer( wxVERTICAL );

	m_canvasXY = new Canvas3D( m_panelXY, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizerXY->Add( m_canvasXY, 1, wxALL|wxEXPAND, 5 );

	wxWrapSizer* wSizerXY;
	wSizerXY = new wxWrapSizer( wxHORIZONTAL, 0 );


	wSizerXY->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticTextXMin2 = new wxStaticText( m_panelXY, wxID_ANY, _("x min:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextXMin2->Wrap( -1 );
	wSizerXY->Add( m_staticTextXMin2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrlXMin2 = new ExtendedTextCtrl( m_panelXY, ID_XMIN, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTER|wxTE_PROCESS_ENTER );
	wSizerXY->Add( m_textCtrlXMin2, 0, wxALL, 5 );

	m_staticTextXMax2 = new wxStaticText( m_panelXY, wxID_ANY, _("x max:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextXMax2->Wrap( -1 );
	wSizerXY->Add( m_staticTextXMax2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrlXMax2 = new ExtendedTextCtrl( m_panelXY, ID_XMAX, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTER|wxTE_PROCESS_ENTER );
	wSizerXY->Add( m_textCtrlXMax2, 0, wxALL, 5 );

	m_staticTextYMin2 = new wxStaticText( m_panelXY, wxID_ANY, _("y min:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextYMin2->Wrap( -1 );
	wSizerXY->Add( m_staticTextYMin2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrlYMin2 = new ExtendedTextCtrl( m_panelXY, ID_YMIN, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTER|wxTE_PROCESS_ENTER );
	wSizerXY->Add( m_textCtrlYMin2, 0, wxALL, 5 );

	m_staticTextYMax2 = new wxStaticText( m_panelXY, wxID_ANY, _("y max:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextYMax2->Wrap( -1 );
	wSizerXY->Add( m_staticTextYMax2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrlYMax2 = new ExtendedTextCtrl( m_panelXY, ID_YMAX, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTER|wxTE_PROCESS_ENTER );
	wSizerXY->Add( m_textCtrlYMax2, 0, wxALL, 5 );


	wSizerXY->Add( 0, 0, 1, wxEXPAND, 5 );


	bSizerXY->Add( wSizerXY, 0, wxEXPAND, 5 );


	m_panelXY->SetSizer( bSizerXY );
	m_panelXY->Layout();
	bSizerXY->Fit( m_panelXY );
	m_notebook->AddPage( m_panelXY, _("z = f(x,y)"), false );
	m_panelXYZ = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerXYZ;
	bSizerXYZ = new wxBoxSizer( wxVERTICAL );

	m_canvasXYZ = new Canvas3D( m_panelXYZ, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizerXYZ->Add( m_canvasXYZ, 1, wxALL|wxEXPAND, 5 );

	wxWrapSizer* wSizerXYZ;
	wSizerXYZ = new wxWrapSizer( wxHORIZONTAL, 0 );


	wSizerXYZ->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticTextXMin3 = new wxStaticText( m_panelXYZ, wxID_ANY, _("x min:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextXMin3->Wrap( -1 );
	wSizerXYZ->Add( m_staticTextXMin3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrlXMin3 = new ExtendedTextCtrl( m_panelXYZ, ID_XMIN, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTER|wxTE_DONTWRAP|wxTE_PROCESS_ENTER );
	wSizerXYZ->Add( m_textCtrlXMin3, 0, wxALL, 5 );

	m_staticTextXMax3 = new wxStaticText( m_panelXYZ, wxID_ANY, _("x max:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextXMax3->Wrap( -1 );
	wSizerXYZ->Add( m_staticTextXMax3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrlXMax3 = new ExtendedTextCtrl( m_panelXYZ, ID_XMAX, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTER|wxTE_PROCESS_ENTER );
	wSizerXYZ->Add( m_textCtrlXMax3, 0, wxALL, 5 );

	m_staticTextYMin3 = new wxStaticText( m_panelXYZ, wxID_ANY, _("y Min:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextYMin3->Wrap( -1 );
	wSizerXYZ->Add( m_staticTextYMin3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrlYMin3 = new ExtendedTextCtrl( m_panelXYZ, ID_YMIN, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTER|wxTE_PROCESS_ENTER );
	wSizerXYZ->Add( m_textCtrlYMin3, 0, wxALL, 5 );

	m_staticTextYMax3 = new wxStaticText( m_panelXYZ, wxID_ANY, _("y max:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextYMax3->Wrap( -1 );
	wSizerXYZ->Add( m_staticTextYMax3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrlYMax3 = new ExtendedTextCtrl( m_panelXYZ, ID_YMAX, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTER|wxTE_PROCESS_ENTER );
	wSizerXYZ->Add( m_textCtrlYMax3, 0, wxALL, 5 );

	m_staticTextZMin3 = new wxStaticText( m_panelXYZ, wxID_ANY, _("z min:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextZMin3->Wrap( -1 );
	wSizerXYZ->Add( m_staticTextZMin3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrlZMin3 = new ExtendedTextCtrl( m_panelXYZ, ID_ZMIN, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTER|wxTE_PROCESS_ENTER );
	wSizerXYZ->Add( m_textCtrlZMin3, 0, wxALL, 5 );

	m_staticTextZMax3 = new wxStaticText( m_panelXYZ, wxID_ANY, _("z max:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextZMax3->Wrap( -1 );
	wSizerXYZ->Add( m_staticTextZMax3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrlZMax3 = new ExtendedTextCtrl( m_panelXYZ, ID_ZMAX, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTER|wxTE_PROCESS_ENTER );
	wSizerXYZ->Add( m_textCtrlZMax3, 0, wxALL, 5 );


	wSizerXYZ->Add( 0, 0, 1, wxEXPAND, 5 );


	bSizerXYZ->Add( wSizerXYZ, 0, wxEXPAND, 5 );


	m_panelXYZ->SetSizer( bSizerXYZ );
	m_panelXYZ->Layout();
	bSizerXYZ->Fit( m_panelXYZ );
	m_notebook->AddPage( m_panelXYZ, _("0 = f(x,y,z)"), false );

	bSizerBottom->Add( m_notebook, 1, wxEXPAND | wxALL, 5 );


	m_panelBottom->SetSizer( bSizerBottom );
	m_panelBottom->Layout();
	bSizerBottom->Fit( m_panelBottom );
	m_splitterHorizontal->SplitHorizontally( m_panelTop, m_panelBottom, 344 );
	bSizerSplitterHorizontal->Add( m_splitterHorizontal, 1, wxEXPAND, 5 );


	m_panelLeft->SetSizer( bSizerSplitterHorizontal );
	m_panelLeft->Layout();
	bSizerSplitterHorizontal->Fit( m_panelLeft );
	m_panelRight = new wxPanel( m_splitterVertical, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizerInfo;
	fgSizerInfo = new wxFlexGridSizer( 8, 1, 0, 0 );
	fgSizerInfo->AddGrowableCol( 0 );
	fgSizerInfo->AddGrowableRow( 1 );
	fgSizerInfo->AddGrowableRow( 3 );
	fgSizerInfo->AddGrowableRow( 5 );
	fgSizerInfo->SetFlexibleDirection( wxBOTH );
	fgSizerInfo->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextVariables = new wxStaticText( m_panelRight, wxID_ANY, _("Variables:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextVariables->Wrap( -1 );
	fgSizerInfo->Add( m_staticTextVariables, 0, wxALL, 5 );

	m_propertyGridVariables = new wxPropertyGrid(m_panelRight, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_SPLITTER_AUTO_CENTER|wxTAB_TRAVERSAL);
	fgSizerInfo->Add( m_propertyGridVariables, 0, wxALL|wxEXPAND, 5 );

	m_staticTextStack = new wxStaticText( m_panelRight, wxID_ANY, _("Stack:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextStack->Wrap( -1 );
	fgSizerInfo->Add( m_staticTextStack, 0, wxALL, 5 );

	m_textCtrlStack = new wxStyledTextCtrl( m_panelRight, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, wxEmptyString );
	m_textCtrlStack->SetUseTabs( true );
	m_textCtrlStack->SetTabWidth( 4 );
	m_textCtrlStack->SetIndent( 4 );
	m_textCtrlStack->SetTabIndents( true );
	m_textCtrlStack->SetBackSpaceUnIndents( true );
	m_textCtrlStack->SetViewEOL( false );
	m_textCtrlStack->SetViewWhiteSpace( false );
	m_textCtrlStack->SetMarginWidth( 2, 0 );
	m_textCtrlStack->SetIndentationGuides( true );
	m_textCtrlStack->SetReadOnly( false );
	m_textCtrlStack->SetMarginType( 1, wxSTC_MARGIN_SYMBOL );
	m_textCtrlStack->SetMarginMask( 1, wxSTC_MASK_FOLDERS );
	m_textCtrlStack->SetMarginWidth( 1, 16);
	m_textCtrlStack->SetMarginSensitive( 1, true );
	m_textCtrlStack->SetProperty( wxT("fold"), wxT("1") );
	m_textCtrlStack->SetFoldFlags( wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED | wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED );
	m_textCtrlStack->SetMarginType( 0, wxSTC_MARGIN_NUMBER );
	m_textCtrlStack->SetMarginWidth( 0, m_textCtrlStack->TextWidth( wxSTC_STYLE_LINENUMBER, wxT("_99999") ) );
	m_textCtrlStack->MarkerDefine( wxSTC_MARKNUM_FOLDER, wxSTC_MARK_BOXPLUS );
	m_textCtrlStack->MarkerSetBackground( wxSTC_MARKNUM_FOLDER, wxColour( wxT("BLACK") ) );
	m_textCtrlStack->MarkerSetForeground( wxSTC_MARKNUM_FOLDER, wxColour( wxT("WHITE") ) );
	m_textCtrlStack->MarkerDefine( wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_BOXMINUS );
	m_textCtrlStack->MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPEN, wxColour( wxT("BLACK") ) );
	m_textCtrlStack->MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPEN, wxColour( wxT("WHITE") ) );
	m_textCtrlStack->MarkerDefine( wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_EMPTY );
	m_textCtrlStack->MarkerDefine( wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_BOXPLUS );
	m_textCtrlStack->MarkerSetBackground( wxSTC_MARKNUM_FOLDEREND, wxColour( wxT("BLACK") ) );
	m_textCtrlStack->MarkerSetForeground( wxSTC_MARKNUM_FOLDEREND, wxColour( wxT("WHITE") ) );
	m_textCtrlStack->MarkerDefine( wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_BOXMINUS );
	m_textCtrlStack->MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPENMID, wxColour( wxT("BLACK") ) );
	m_textCtrlStack->MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPENMID, wxColour( wxT("WHITE") ) );
	m_textCtrlStack->MarkerDefine( wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY );
	m_textCtrlStack->MarkerDefine( wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_EMPTY );
	m_textCtrlStack->SetSelBackground( true, wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );
	m_textCtrlStack->SetSelForeground( true, wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHTTEXT ) );
	fgSizerInfo->Add( m_textCtrlStack, 1, wxEXPAND | wxALL, 5 );

	m_staticTextAsm = new wxStaticText( m_panelRight, wxID_ANY, _("Compiled instructions:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextAsm->Wrap( -1 );
	fgSizerInfo->Add( m_staticTextAsm, 0, wxALL, 5 );

	m_textCtrlAsm = new wxStyledTextCtrl( m_panelRight, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, wxEmptyString );
	m_textCtrlAsm->SetUseTabs( true );
	m_textCtrlAsm->SetTabWidth( 4 );
	m_textCtrlAsm->SetIndent( 4 );
	m_textCtrlAsm->SetTabIndents( true );
	m_textCtrlAsm->SetBackSpaceUnIndents( true );
	m_textCtrlAsm->SetViewEOL( false );
	m_textCtrlAsm->SetViewWhiteSpace( false );
	m_textCtrlAsm->SetMarginWidth( 2, 0 );
	m_textCtrlAsm->SetIndentationGuides( true );
	m_textCtrlAsm->SetReadOnly( false );
	m_textCtrlAsm->SetMarginType( 1, wxSTC_MARGIN_SYMBOL );
	m_textCtrlAsm->SetMarginMask( 1, wxSTC_MASK_FOLDERS );
	m_textCtrlAsm->SetMarginWidth( 1, 16);
	m_textCtrlAsm->SetMarginSensitive( 1, true );
	m_textCtrlAsm->SetProperty( wxT("fold"), wxT("1") );
	m_textCtrlAsm->SetFoldFlags( wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED | wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED );
	m_textCtrlAsm->SetMarginType( 0, wxSTC_MARGIN_NUMBER );
	m_textCtrlAsm->SetMarginWidth( 0, m_textCtrlAsm->TextWidth( wxSTC_STYLE_LINENUMBER, wxT("_99999") ) );
	m_textCtrlAsm->MarkerDefine( wxSTC_MARKNUM_FOLDER, wxSTC_MARK_BOXPLUS );
	m_textCtrlAsm->MarkerSetBackground( wxSTC_MARKNUM_FOLDER, wxColour( wxT("BLACK") ) );
	m_textCtrlAsm->MarkerSetForeground( wxSTC_MARKNUM_FOLDER, wxColour( wxT("WHITE") ) );
	m_textCtrlAsm->MarkerDefine( wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_BOXMINUS );
	m_textCtrlAsm->MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPEN, wxColour( wxT("BLACK") ) );
	m_textCtrlAsm->MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPEN, wxColour( wxT("WHITE") ) );
	m_textCtrlAsm->MarkerDefine( wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_EMPTY );
	m_textCtrlAsm->MarkerDefine( wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_BOXPLUS );
	m_textCtrlAsm->MarkerSetBackground( wxSTC_MARKNUM_FOLDEREND, wxColour( wxT("BLACK") ) );
	m_textCtrlAsm->MarkerSetForeground( wxSTC_MARKNUM_FOLDEREND, wxColour( wxT("WHITE") ) );
	m_textCtrlAsm->MarkerDefine( wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_BOXMINUS );
	m_textCtrlAsm->MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPENMID, wxColour( wxT("BLACK") ) );
	m_textCtrlAsm->MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPENMID, wxColour( wxT("WHITE") ) );
	m_textCtrlAsm->MarkerDefine( wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY );
	m_textCtrlAsm->MarkerDefine( wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_EMPTY );
	m_textCtrlAsm->SetSelBackground( true, wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );
	m_textCtrlAsm->SetSelForeground( true, wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHTTEXT ) );
	fgSizerInfo->Add( m_textCtrlAsm, 1, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizerCheckbox;
	bSizerCheckbox = new wxBoxSizer( wxHORIZONTAL );

	m_checkBoxAutorun = new wxCheckBox( m_panelRight, ID_AUTORUN, _("autorun"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBoxAutorun->SetValue(true);
	bSizerCheckbox->Add( m_checkBoxAutorun, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );


	bSizerCheckbox->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticTextMaxSteps = new wxStaticText( m_panelRight, wxID_ANY, _("max steps:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextMaxSteps->Wrap( -1 );
	bSizerCheckbox->Add( m_staticTextMaxSteps, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrlMaxSteps = new ExtendedTextCtrl( m_panelRight, ID_MAXSTEPS, _("10000"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrlMaxSteps->SetToolTip( _("Maximum number of instructions to execute. Prevents infinite loops.") );

	bSizerCheckbox->Add( m_textCtrlMaxSteps, 0, wxALL, 5 );


	fgSizerInfo->Add( bSizerCheckbox, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizerButtons;
	fgSizerButtons = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizerButtons->AddGrowableCol( 0 );
	fgSizerButtons->AddGrowableCol( 1 );
	fgSizerButtons->AddGrowableRow( 0 );
	fgSizerButtons->AddGrowableRow( 1 );
	fgSizerButtons->SetFlexibleDirection( wxBOTH );
	fgSizerButtons->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_buttonRun = new wxButton( m_panelRight, ID_RUN, _("run"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizerButtons->Add( m_buttonRun, 0, wxALL|wxEXPAND, 5 );

	m_buttonReset = new wxButton( m_panelRight, ID_STOP, _("stop/reset"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizerButtons->Add( m_buttonReset, 0, wxALL|wxEXPAND, 5 );

	m_buttonStepAsm = new wxButton( m_panelRight, ID_STEPASM, _("step asm"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizerButtons->Add( m_buttonStepAsm, 0, wxALL|wxEXPAND, 5 );

	m_buttonStepExpr = new wxButton( m_panelRight, ID_STEPINSTRUCTION, _("step expr"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizerButtons->Add( m_buttonStepExpr, 0, wxALL|wxEXPAND, 5 );


	fgSizerInfo->Add( fgSizerButtons, 1, wxEXPAND, 5 );


	m_panelRight->SetSizer( fgSizerInfo );
	m_panelRight->Layout();
	fgSizerInfo->Fit( m_panelRight );
	m_splitterVertical->SplitVertically( m_panelLeft, m_panelRight, 1040 );
	bSizerSplitterVertical->Add( m_splitterVertical, 1, wxEXPAND, 5 );


	this->SetSizer( bSizerSplitterVertical );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GUIFrameCalculator::OnClose ) );
	m_notebook->Connect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( GUIFrameCalculator::OnNotebookPageChanged ), NULL, this );
	m_notebook->Connect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGING, wxNotebookEventHandler( GUIFrameCalculator::OnNotebookPageChanging ), NULL, this );
	m_textCtrlXMin->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameCalculator::OnKillFocus ), NULL, this );
	m_textCtrlXMin->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameCalculator::OnTextEnter ), NULL, this );
	m_textCtrlXMax->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameCalculator::OnKillFocus ), NULL, this );
	m_textCtrlXMax->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameCalculator::OnTextEnter ), NULL, this );
	m_textCtrlXMin2->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameCalculator::OnKillFocus ), NULL, this );
	m_textCtrlXMin2->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameCalculator::OnTextEnter ), NULL, this );
	m_textCtrlXMax2->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameCalculator::OnKillFocus ), NULL, this );
	m_textCtrlXMax2->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameCalculator::OnTextEnter ), NULL, this );
	m_textCtrlYMin2->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameCalculator::OnKillFocus ), NULL, this );
	m_textCtrlYMin2->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameCalculator::OnTextEnter ), NULL, this );
	m_textCtrlYMax2->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameCalculator::OnKillFocus ), NULL, this );
	m_textCtrlYMax2->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameCalculator::OnTextEnter ), NULL, this );
	m_textCtrlXMin3->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameCalculator::OnKillFocus ), NULL, this );
	m_textCtrlXMin3->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameCalculator::OnTextEnter ), NULL, this );
	m_textCtrlXMax3->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameCalculator::OnKillFocus ), NULL, this );
	m_textCtrlXMax3->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameCalculator::OnTextEnter ), NULL, this );
	m_textCtrlYMin3->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameCalculator::OnKillFocus ), NULL, this );
	m_textCtrlYMin3->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameCalculator::OnTextEnter ), NULL, this );
	m_textCtrlYMax3->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameCalculator::OnKillFocus ), NULL, this );
	m_textCtrlYMax3->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameCalculator::OnTextEnter ), NULL, this );
	m_textCtrlZMin3->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameCalculator::OnKillFocus ), NULL, this );
	m_textCtrlZMin3->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameCalculator::OnTextEnter ), NULL, this );
	m_textCtrlZMax3->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameCalculator::OnKillFocus ), NULL, this );
	m_textCtrlZMax3->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameCalculator::OnTextEnter ), NULL, this );
	m_propertyGridVariables->Connect( wxEVT_PG_CHANGED, wxPropertyGridEventHandler( GUIFrameCalculator::OnPropertyGridChanged ), NULL, this );
	m_propertyGridVariables->Connect( wxEVT_PG_CHANGING, wxPropertyGridEventHandler( GUIFrameCalculator::OnPropertyGridChanging ), NULL, this );
	m_checkBoxAutorun->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GUIFrameCalculator::OnCheckBox ), NULL, this );
	m_textCtrlMaxSteps->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameCalculator::OnKillFocus ), NULL, this );
	m_textCtrlMaxSteps->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameCalculator::OnTextEnter ), NULL, this );
	m_buttonRun->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameCalculator::OnButton ), NULL, this );
	m_buttonReset->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameCalculator::OnButton ), NULL, this );
	m_buttonStepAsm->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameCalculator::OnButton ), NULL, this );
	m_buttonStepExpr->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameCalculator::OnButton ), NULL, this );
}

GUIFrameCalculator::~GUIFrameCalculator()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GUIFrameCalculator::OnClose ) );
	m_notebook->Disconnect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( GUIFrameCalculator::OnNotebookPageChanged ), NULL, this );
	m_notebook->Disconnect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGING, wxNotebookEventHandler( GUIFrameCalculator::OnNotebookPageChanging ), NULL, this );
	m_textCtrlXMin->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameCalculator::OnKillFocus ), NULL, this );
	m_textCtrlXMin->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameCalculator::OnTextEnter ), NULL, this );
	m_textCtrlXMax->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameCalculator::OnKillFocus ), NULL, this );
	m_textCtrlXMax->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameCalculator::OnTextEnter ), NULL, this );
	m_textCtrlXMin2->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameCalculator::OnKillFocus ), NULL, this );
	m_textCtrlXMin2->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameCalculator::OnTextEnter ), NULL, this );
	m_textCtrlXMax2->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameCalculator::OnKillFocus ), NULL, this );
	m_textCtrlXMax2->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameCalculator::OnTextEnter ), NULL, this );
	m_textCtrlYMin2->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameCalculator::OnKillFocus ), NULL, this );
	m_textCtrlYMin2->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameCalculator::OnTextEnter ), NULL, this );
	m_textCtrlYMax2->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameCalculator::OnKillFocus ), NULL, this );
	m_textCtrlYMax2->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameCalculator::OnTextEnter ), NULL, this );
	m_textCtrlXMin3->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameCalculator::OnKillFocus ), NULL, this );
	m_textCtrlXMin3->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameCalculator::OnTextEnter ), NULL, this );
	m_textCtrlXMax3->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameCalculator::OnKillFocus ), NULL, this );
	m_textCtrlXMax3->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameCalculator::OnTextEnter ), NULL, this );
	m_textCtrlYMin3->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameCalculator::OnKillFocus ), NULL, this );
	m_textCtrlYMin3->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameCalculator::OnTextEnter ), NULL, this );
	m_textCtrlYMax3->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameCalculator::OnKillFocus ), NULL, this );
	m_textCtrlYMax3->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameCalculator::OnTextEnter ), NULL, this );
	m_textCtrlZMin3->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameCalculator::OnKillFocus ), NULL, this );
	m_textCtrlZMin3->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameCalculator::OnTextEnter ), NULL, this );
	m_textCtrlZMax3->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameCalculator::OnKillFocus ), NULL, this );
	m_textCtrlZMax3->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameCalculator::OnTextEnter ), NULL, this );
	m_propertyGridVariables->Disconnect( wxEVT_PG_CHANGED, wxPropertyGridEventHandler( GUIFrameCalculator::OnPropertyGridChanged ), NULL, this );
	m_propertyGridVariables->Disconnect( wxEVT_PG_CHANGING, wxPropertyGridEventHandler( GUIFrameCalculator::OnPropertyGridChanging ), NULL, this );
	m_checkBoxAutorun->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( GUIFrameCalculator::OnCheckBox ), NULL, this );
	m_textCtrlMaxSteps->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GUIFrameCalculator::OnKillFocus ), NULL, this );
	m_textCtrlMaxSteps->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GUIFrameCalculator::OnTextEnter ), NULL, this );
	m_buttonRun->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameCalculator::OnButton ), NULL, this );
	m_buttonReset->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameCalculator::OnButton ), NULL, this );
	m_buttonStepAsm->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameCalculator::OnButton ), NULL, this );
	m_buttonStepExpr->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrameCalculator::OnButton ), NULL, this );

}
