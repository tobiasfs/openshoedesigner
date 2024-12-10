///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/docview.h>
#include <wx/config.h>
#include "ExtendedTextCtrl.h"

#include <wx/string.h>
#include <wx/menu.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/tglbtn.h>
#include <wx/radiobox.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/statbox.h>
#include <wx/statbmp.h>
#include <wx/panel.h>
#include <wx/filepicker.h>
#include <wx/choicebk.h>
#include <wx/slider.h>
#include <wx/choice.h>
#include <wx/dataview.h>
#include <wx/treelist.h>
#include <wx/listbox.h>
#include <wx/notebook.h>
#include "Canvas3D.h"
#include "PanelPattern.h"
#include "PanelSupport.h"
#include "PanelWalkcycle.h"
#include <wx/checkbox.h>
#include "PanelPlotSimple.h"
#include <wx/splitter.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/grid.h>
#include "CanvasFootModel.h"
#include <wx/radiobut.h>
#include <wx/dialog.h>
#include <wx/listctrl.h>
#include <wx/spinbutt.h>
#include "PanelAnisotropy.h"

///////////////////////////////////////////////////////////////////////////

#define ID_QUICKSETUPMEASUREMENTS 1000
#define ID_USEFOOTMEASUREMENTS 1001
#define ID_USEFOOTSCAN 1002
#define ID_MEASUREMENTSYMMETRY 1003
#define ID_COPYACTIVETOINACTIVE 1004
#define ID_COPYRIGHTTOLEFT 1005
#define ID_COPYLEFTTORIGHT 1006
#define ID_USEBONEBASEDMODEL 1007
#define ID_USELASTBASEDMODEL 1008
#define ID_EDITBONEMODEL 1009
#define ID_LOADFOOTMODEL 1010
#define ID_SAVEFOOTMODEL 1011
#define ID_CONSTRUCTIONEXPERIMENTAL 1012
#define ID_CONSTRUCTIONWELDED 1013
#define ID_CONSTRUCTIONCEMENTED 1014
#define ID_CONSTRUCTIONMOLDED 1015
#define ID_CONSTRUCTIONDUTCH 1016
#define ID_CONSTRUCTIONGETA 1017
#define ID_LOADPATTERN 1018
#define ID_SAVEPATTERN 1019
#define ID_STEREO3D 1020
#define ID_SHOWLEFT 1021
#define ID_SHOWRIGHT 1022
#define ID_SHOWBONES 1023
#define ID_SHOWSKIN 1024
#define ID_SHOWLEG 1025
#define ID_SHOWLAST 1026
#define ID_SHOWINSOLE 1027
#define ID_SHOWSOLE 1028
#define ID_SHOWUPPER 1029
#define ID_SHOWCUTAWAY 1030
#define ID_SHOWFLOOR 1031
#define ID_SHOWCOORDINATESYSTEM 1032
#define ID_SHOWBACKGROUND 1033
#define ID_EDITLEFT 1034
#define ID_EDITRIGHT 1035
#define ID_EDITSIDE 1036
#define ID_MEASUREMENTSOURCE 1037
#define ID_MEASUREMENT_FOOTLENGTH 1038
#define ID_MEASUREMENT_BALLWIDTH 1039
#define ID_MEASUREMENT_BIGTOEGIRTH 1040
#define ID_MEASUREMENT_LITTLETOEGIRTH 1041
#define ID_MEASUREMENT_WAISTGIRTH 1042
#define ID_MEASUREMENT_HEELGIRTH 1043
#define ID_MEASUREMENT_HEELWIDTH 1044
#define ID_MEASUREMENT_ANGLEMIXING 1045
#define ID_MEASUREMENT_LEGLENGTHDIFFERENCE 1046
#define ID_IMAGEFOOT 1047
#define ID_MODELTYPE 1048
#define ID_LASTFILENAME 1049
#define ID_MEASUREMENT_BELOWCRUTCHGIRTH 1050
#define ID_MEASUREMENT_BELOWCRUTCHLEVEL 1051
#define ID_MEASUREMENT_MIDDLEOFCALFGIRTH 1052
#define ID_MEASUREMENT_MIDDLEOFCALFLEVEL 1053
#define ID_MEASUREMENT_ABOVEKNEEGIRTH 1054
#define ID_MEASUREMENT_ABOVEKNEELEVEL 1055
#define ID_MEASUREMENT_OVERKNEECAPGIRTH 1056
#define ID_MEASUREMENT_OVERKNEECAPLEVEL 1057
#define ID_MEASUREMENT_BELOWKNEEGIRTH 1058
#define ID_MEASUREMENT_BELOWKNEELEVEL 1059
#define ID_MEASUREMENT_MIDDLEOFSHANKGIRTH 1060
#define ID_MEASUREMENT_MIDDLEOFSHANKLEVEL 1061
#define ID_MEASUREMENT_ABOVEANKLEGIRTH 1062
#define ID_MEASUREMENT_ABOVEANKLELEVEL 1063
#define ID_MEASUREMENT_OVERANKLEBONEGIRTH 1064
#define ID_MEASUREMENT_OVERANKLEBONELEVEL 1065
#define ID_BIGTOEANGLE 1066
#define ID_LITTLETOEANGLE 1067
#define ID_BALLMEASUREMENTANGLE 1068
#define ID_HEELDIRECTIONANGLE 1069
#define ID_TIPSHARPNESS 1070
#define ID_EXTRALENGTH 1071
#define ID_FOOTCOMPRESSION 1072
#define ID_PRESETSHOETYPE 1073
#define ID_HEELHEIGHT 1074
#define ID_BALLHEIGHT 1075
#define ID_HEELPITCH 1076
#define ID_TOESPRING 1077
#define ID_PRESETSHOEHEIGHT 1078
#define ID_UPPERLEVEL 1079
#define ID_SLIDERTOESUPPORTRADIUS 1080
#define ID_SLIDERTOESUPPORTOFFSET 1081
#define ID_GENERATOR 1082
#define ID_EDITHEELSHAPE 1083
#define ID_EDITPLATFORMSHAPE 1084
#define ID_EDITSOLESHAPE 1085
#define ID_EDITWELTEDGE 1086
#define ID_EDITTHICKNESS 1087
#define ID_ADDBRIDGE 1088
#define ID_DELETEBRIDGE 1089
#define ID_PATTERNADDLINE 1090
#define ID_PATTERNADDPATCH 1091
#define ID_PATTERNADDSTITCHING 1092
#define ID_PATTERNADDPADDING 1093
#define ID_PATTERNADDCOORDINATESYSTEM 1094
#define ID_PATTERNADDLACES 1095
#define ID_PATTERNADDACCESSORY 1096
#define ID_EXPORTFLATTENING 1097
#define ID_TESTSTITCH 1098
#define ID_GRIDLENGTH 1099
#define ID_GRIDDIAMETER 1100
#define ID_GRIDSKIN 1101
#define ID_SPINSCALE 1102
#define ID_SPINOFFSHOR 1103
#define ID_SPINOFFSVERT 1104
#define ID_CLOSE 1105
#define ID_POINTADD 1106
#define ID_POINTREMOVE 1107

///////////////////////////////////////////////////////////////////////////////
/// Class GUIFrameMain
///////////////////////////////////////////////////////////////////////////////
class GUIFrameMain : public wxDocChildFrame
{
	private:

	protected:
		wxMenuBar* m_menubar;
		wxMenu* m_menuFile;
		wxMenu* m_menuEdit;
		wxMenu* m_menuFoot;
		wxMenu* m_menuFootParameters;
		wxMenu* m_menuMeasurementsCopy;
		wxMenu* m_menuFootModel;
		wxMenu* m_menuBoneModel;
		wxMenu* m_menuLast;
		wxMenu* m_menuShoe;
		wxMenu* m_menuConstruction;
		wxMenu* m_menuPattern;
		wxMenu* m_menuFabric;
		wxMenu* m_menuGeometry;
		wxMenu* m_menuView;
		wxMenu* m_menuPreferences;
		wxMenu* m_menuHelp;
		wxSplitterWindow* m_splitter;
		wxPanel* m_panelConfig;
		wxNotebook* m_notebook;
		wxPanel* m_panelPageFoot;
		wxToggleButton* m_toggleBtnEditLeft;
		wxToggleButton* m_toggleBtnEditRight;
		wxRadioBox* m_radioBoxMeasurementsSide;
		wxChoicebook* m_choicebookMeasurement;
		wxPanel* m_panelMeasurementBased;
		wxStaticText* m_staticTextFootLength;
		ExtendedTextCtrl* m_textCtrlFootLength;
		wxStaticText* m_staticTextBallWidth;
		ExtendedTextCtrl* m_textCtrlBallWidth;
		wxStaticText* m_staticTextBallGirth;
		ExtendedTextCtrl* m_textCtrlBigToeGirth;
		wxStaticText* m_staticTextWaistGirth;
		ExtendedTextCtrl* m_textCtrlLittleToeGirth;
		wxStaticText* m_staticTextInsteGirth;
		ExtendedTextCtrl* m_textCtrlWaistGirth;
		wxStaticText* m_staticTextLongHeelGirth;
		ExtendedTextCtrl* m_textCtrlHeelGirth;
		wxStaticText* m_staticTextShortHeelGirth;
		ExtendedTextCtrl* m_textCtrlHeelWidth;
		wxStaticText* m_staticTextAngleMixing;
		ExtendedTextCtrl* m_textCtrlAngleMixing;
		wxStaticText* m_staticTextLegLengthDifference;
		ExtendedTextCtrl* m_textCtrlLegLengthDifference;
		wxButton* m_buttonQuickSetup;
		wxStaticText* m_staticTextShoeSizeEU;
		ExtendedTextCtrl* m_textCtrlShoeSizeEU;
		wxStaticText* m_staticTextShoeSizeUS;
		ExtendedTextCtrl* m_textCtrlShoeSizeUS;
		wxStaticText* m_staticTextShoeSizeUK;
		ExtendedTextCtrl* m_textCtrlShoeSizeUK;
		wxStaticText* m_staticTextShoeSizeCN;
		ExtendedTextCtrl* m_textCtrlShoeSizeCN;
		wxStaticText* m_staticTextShoeSizeJP;
		ExtendedTextCtrl* m_textCtrlShoeSizeJP;
		wxStaticText* m_staticTextShoeSizeAU;
		ExtendedTextCtrl* m_textCtrlShoeSizeAU;
		wxStaticBitmap* m_bitmapFoot;
		wxPanel* m_panelScanBased;
		wxStaticText* m_staticTextScanFile;
		wxFilePickerCtrl* m_filePickerScanFile;
		wxChoicebook* m_choicebookFootModel;
		wxPanel* m_panelBoneBasedModel;
		wxButton* m_buttonEditBoneModel;
		wxPanel* m_panelLastBasedModel;
		wxStaticText* m_staticTextLastModel;
		wxFilePickerCtrl* m_filePickerLastModel;
		wxPanel* m_panelPageLeg;
		wxToggleButton* m_toggleBtnEditLeft1;
		wxToggleButton* m_toggleBtnEditRight1;
		wxStaticBitmap* m_bitmapLeg;
		wxStaticText* m_staticTextGirth;
		wxStaticText* m_staticTextLevel;
		ExtendedTextCtrl* m_textCtrlBelowCrutchGirth;
		ExtendedTextCtrl* m_textCtrlBelowCrutchLevel;
		ExtendedTextCtrl* m_textCtrlMiddleOfCalfGirth;
		ExtendedTextCtrl* m_textCtrlMiddleOfCalfLevel;
		ExtendedTextCtrl* m_textCtrlAboveKneeGirth;
		ExtendedTextCtrl* m_textCtrlAboveKneeLevel;
		ExtendedTextCtrl* m_textCtrlOverKneeCapGirth;
		ExtendedTextCtrl* m_textCtrlOverKneeCapLevel;
		ExtendedTextCtrl* m_textCtrlBelowKneeGirth;
		ExtendedTextCtrl* m_textCtrlBelowKneeLevel;
		ExtendedTextCtrl* m_textCtrlMiddleOfShankGirth;
		ExtendedTextCtrl* m_textCtrlMiddleOfShankLevel;
		ExtendedTextCtrl* m_textCtrlAboveAnkleGirth;
		ExtendedTextCtrl* m_textCtrlAboveAnkleLevel;
		ExtendedTextCtrl* m_textCtrlOverAnkleBoneGirth;
		ExtendedTextCtrl* m_textCtrlOverAnkleBoneLevel;
		wxPanel* m_panelPageInsole;
		wxStaticText* m_staticTextTipSymmetry;
		ExtendedTextCtrl* m_textCtrlBigToeAngle;
		wxStaticText* m_staticTextTipPointedness;
		ExtendedTextCtrl* m_textCtrlLittleToeAngle;
		wxStaticText* m_staticText57;
		ExtendedTextCtrl* m_textCtrlBallMeasurementAngle;
		wxStaticText* m_staticText58;
		ExtendedTextCtrl* m_textCtrlHeelDirectionAngle;
		wxStaticText* m_staticTextTipSharpness;
		wxSlider* m_sliderTipSharpness;
		wxStaticText* m_staticTextExtraLength;
		ExtendedTextCtrl* m_textCtrlExtraLength;
		wxStaticText* m_staticTextFootCompression;
		ExtendedTextCtrl* m_textCtrlFootCompression;
		wxPanel* m_panelPageShoe;
		wxChoice* m_choiceShoeType;
		wxStaticText* m_staticTextHeelHeight;
		ExtendedTextCtrl* m_textCtrlHeelHeight;
		wxStaticText* m_staticTextBallHeight;
		ExtendedTextCtrl* m_textCtrlBallHeight;
		wxStaticText* m_staticTextHeelPitch;
		ExtendedTextCtrl* m_textCtrlHeelPitch;
		wxStaticText* m_staticTextToeSpring;
		ExtendedTextCtrl* m_textCtrlToeSpring;
		wxChoice* m_choiceShoeHeight;
		wxStaticText* m_staticTextUpperLevel;
		ExtendedTextCtrl* m_textCtrlUpperLevel;
		wxPanel* m_panelPageSole;
		wxSlider* m_sliderSupportHeelRadius;
		wxSlider* m_sliderSupportHeelOffset;
		wxSlider* m_sliderSupportToeRadius;
		wxSlider* m_sliderSupportToeOffset;
		wxStaticText* m_staticTextConstruction;
		wxChoicebook* m_choicebookConstruction;
		wxPanel* m_panelExperimental;
		wxChoicebook* m_choicebookHeelSeparation;
		wxPanel* m_panelHeelSeparated;
		wxButton* m_buttonEditHeelShape;
		wxButton* m_buttonEditPlatformShape;
		wxPanel* m_panelSingleSole;
		wxButton* m_buttonEditSoleShape;
		wxPanel* m_panelWelted;
		wxButton* m_buttonEditHeelShape2;
		wxButton* m_buttonEditWeltEdge;
		wxPanel* m_panelCemented;
		wxButton* m_buttonEditSoleShape2;
		wxPanel* m_panelMolded;
		wxPanel* m_panelClog;
		wxButton* m_buttonEditThickness;
		wxButton* m_buttonEditSoleShape3;
		wxPanel* m_panelGeta;
		wxStaticText* m_staticTextBridge;
		wxButton* m_buttonAddBridge;
		wxButton* m_buttonDeleteBridge;
		wxDataViewListCtrl* m_dataViewListCtrlBridges;
		wxDataViewColumn* m_dataViewListColumnBridgePosition;
		wxDataViewColumn* m_dataViewListColumnBridgeWidth;
		wxPanel* m_panelPagePattern;
		wxTreeListCtrl* m_treeListCtrlPattern;
		wxButton* m_buttonOnPatternAddLine;
		wxButton* m_buttonOnPatternAddPatch;
		wxButton* m_buttonOnPatternAddStitching;
		wxButton* m_buttonOnPatternAddPadding;
		wxButton* m_buttonOnPatternAddCoordinateSystem;
		wxButton* m_buttonOnPatternAddLaces;
		wxButton* m_buttonOnPatternAddAccessory;
		wxListBox* m_listBoxFabric;
		wxPanel* m_panelPageFlattening;
		wxButton* m_buttonExportFlattening;
		wxButton* m_buttonTestStitch;
		wxPanel* m_panelCanvas;
		Canvas3D * m_canvas3D;
		PanelPattern* m_panelPattern;
		wxPanel* m_panelCheck;
		PanelSupport* m_panelSupport;
		PanelWalkcycle* m_panelCycle;
		wxCheckBox* m_checkBoxLockAnkle;
		wxChoice* m_choiceDisplay;
		PanelPlotSimple* m_panelPlot;
		wxStatusBar* m_statusBar;

		// Virtual event handlers, override them in your derived class
		virtual void OnClose( wxCloseEvent& event ) = 0;
		virtual void OnQuickSetupMeasurements( wxCommandEvent& event ) = 0;
		virtual void OnChangeModel( wxCommandEvent& event ) = 0;
		virtual void OnCopyMeasurements( wxCommandEvent& event ) = 0;
		virtual void OnLoadFootSTL( wxCommandEvent& event ) = 0;
		virtual void OnEditBoneModel( wxCommandEvent& event ) = 0;
		virtual void OnLoadBoneModel( wxCommandEvent& event ) = 0;
		virtual void OnSaveBoneModel( wxCommandEvent& event ) = 0;
		virtual void OnConstructionSelection( wxCommandEvent& event ) = 0;
		virtual void OnLoadPattern( wxCommandEvent& event ) = 0;
		virtual void OnSavePattern( wxCommandEvent& event ) = 0;
		virtual void OnSaveLast( wxCommandEvent& event ) = 0;
		virtual void OnSaveInsole( wxCommandEvent& event ) = 0;
		virtual void OnSaveSole( wxCommandEvent& event ) = 0;
		virtual void OnSaveCutaway( wxCommandEvent& event ) = 0;
		virtual void OnPackZip( wxCommandEvent& event ) = 0;
		virtual void OnToggleStereo3D( wxCommandEvent& event ) = 0;
		virtual void OnViewChange( wxCommandEvent& event ) = 0;
		virtual void OnSetupBackgroundImages( wxCommandEvent& event ) = 0;
		virtual void OnDebugParser( wxCommandEvent& event ) = 0;
		virtual void OnMouseWheel( wxMouseEvent& event ) = 0;
		virtual void OnToggleButton( wxCommandEvent& event ) = 0;
		virtual void OnPageChange( wxChoicebookEvent& event ) = 0;
		virtual void OnKillFocus( wxFocusEvent& event ) = 0;
		virtual void OnSetFocus( wxFocusEvent& event ) = 0;
		virtual void OnTextEnter( wxCommandEvent& event ) = 0;
		virtual void OnSize( wxSizeEvent& event ) = 0;
		virtual void OnFileChangedScanFile( wxFileDirPickerEvent& event ) = 0;
		virtual void OnFileChangedLastFile( wxFileDirPickerEvent& event ) = 0;
		virtual void OnScroll( wxScrollEvent& event ) = 0;
		virtual void OnChoice( wxCommandEvent& event ) = 0;
		virtual void OnEditShape( wxCommandEvent& event ) = 0;
		virtual void OnAddBridge( wxCommandEvent& event ) = 0;
		virtual void OnDeleteBridge( wxCommandEvent& event ) = 0;
		virtual void OnListCtrlOnSelectionChanged( wxDataViewEvent& event ) = 0;
		virtual void OnPatternSelect( wxTreeListEvent& event ) = 0;
		virtual void OnPatternAdd( wxCommandEvent& event ) = 0;
		virtual void OnPatternSelectFabric( wxCommandEvent& event ) = 0;
		virtual void On3DSelect( wxMouseEvent& event ) = 0;
		virtual void OnToggleAnkleLock( wxCommandEvent& event ) = 0;
		virtual void OnChoiceDisplay( wxCommandEvent& event ) = 0;


	public:

		GUIFrameMain(wxDocument* doc, wxView* view, wxDocParentFrame* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Open Shoe Designer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1000,800 ), long style = wxDEFAULT_FRAME_STYLE|wxRESIZE_BORDER|wxTAB_TRAVERSAL );

		~GUIFrameMain();

		void m_splitterOnIdle( wxIdleEvent& )
		{
			m_splitter->SetSashPosition( 550 );
			m_splitter->Disconnect( wxEVT_IDLE, wxIdleEventHandler( GUIFrameMain::m_splitterOnIdle ), NULL, this );
		}

};

///////////////////////////////////////////////////////////////////////////////
/// Class GUIFrameBoneModel
///////////////////////////////////////////////////////////////////////////////
class GUIFrameBoneModel : public wxFrame
{
	private:

	protected:
		wxNotebook* m_notebook;
		wxPanel* m_panelBone;
		wxGrid* m_gridBoneLength;
		wxPanel* m_panelBoneDiameter;
		wxGrid* m_gridBoneDiameter;
		wxPanel* m_panelSkinThickness;
		wxGrid* m_gridSkinThickness;
		wxMenuBar* m_menubar;
		wxMenu* m_menuFile;
		wxMenu* m_menuHelp;

		// Virtual event handlers, override them in your derived class
		virtual void OnCellChange( wxGridEvent& event ) { event.Skip(); }


	public:

		GUIFrameBoneModel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Bone Model Setup"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 672,458 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~GUIFrameBoneModel();

};

///////////////////////////////////////////////////////////////////////////////
/// Class GUIFrameEditorFootModel
///////////////////////////////////////////////////////////////////////////////
class GUIFrameEditorFootModel : public wxFrame
{
	private:

	protected:
		CanvasFootModel * m_canvasFootModel;
		wxDataViewListCtrl* m_dataViewListCtrl2;
		wxDataViewColumn* m_dataViewListColumn4;
		wxDataViewColumn* m_dataViewListColumn5;
		wxDataViewColumn* m_dataViewListColumn6;
		wxMenuBar* m_menubar;
		wxMenu* m_menu14;

		// Virtual event handlers, override them in your derived class
		virtual void OnLoad( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSave( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClose( wxCommandEvent& event ) { event.Skip(); }


	public:

		GUIFrameEditorFootModel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Foot model editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 715,351 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~GUIFrameEditorFootModel();

};

///////////////////////////////////////////////////////////////////////////////
/// Class GUIDialogQuickInitFoot
///////////////////////////////////////////////////////////////////////////////
class GUIDialogQuickInitFoot : public wxDialog
{
	private:

	protected:
		wxStaticText* m_staticTextShoeSize;
		ExtendedTextCtrl* m_textCtrlShoeSize;
		wxChoice* m_choiceUnit;
		wxRadioButton* m_radioBtnSmall;
		wxRadioButton* m_radioBtnMedium;
		wxRadioButton* m_radioBtnWide;
		wxButton* m_buttonInit;
		wxButton* m_buttonAbort;

		// Virtual event handlers, override them in your derived class
		virtual void OnTextEnter( wxCommandEvent& event ) = 0;
		virtual void OnClose( wxCommandEvent& event ) = 0;


	public:

		GUIDialogQuickInitFoot( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Initialize Settings"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION|wxCLOSE_BOX|wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP|wxSYSTEM_MENU );

		~GUIDialogQuickInitFoot();

};

///////////////////////////////////////////////////////////////////////////////
/// Class GUIFrameDebugParser
///////////////////////////////////////////////////////////////////////////////
class GUIFrameDebugParser : public wxFrame
{
	private:

	protected:
		wxStaticText* m_staticText10;
		ExtendedTextCtrl* m_textCtrlExpression;
		wxStaticText* m_staticText11;
		ExtendedTextCtrl* m_textCtrlNumber;
		wxStaticText* m_staticText12;
		ExtendedTextCtrl* m_textCtrlError;
		wxStaticText* m_staticText14;
		ExtendedTextCtrl* m_textCtrlUnit;
		wxStaticText* m_staticText13;
		ExtendedTextCtrl* m_textCtrlSIBase;

		// Virtual event handlers, override them in your derived class
		virtual void OnCloseX( wxCloseEvent& event ) = 0;
		virtual void OnText( wxCommandEvent& event ) = 0;


	public:

		GUIFrameDebugParser( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Test Parser and Units"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_FRAME_STYLE|wxRESIZE_BORDER|wxTAB_TRAVERSAL );

		~GUIFrameDebugParser();

};

///////////////////////////////////////////////////////////////////////////////
/// Class GUIDialogFormulaEditor
///////////////////////////////////////////////////////////////////////////////
class GUIDialogFormulaEditor : public wxDialog
{
	private:

	protected:
		wxDataViewListCtrl* m_dataViewListCtrl;
		wxDataViewColumn* m_dataViewListColumnVariable;
		wxDataViewColumn* m_dataViewListColumnFormula;
		wxDataViewColumn* m_dataViewListColumnValue;
		wxStaticText* m_staticTextVariableName;
		ExtendedTextCtrl* m_textCtrlFormula;
		ExtendedTextCtrl* m_textCtrlUnit;
		ExtendedTextCtrl* m_textCtrlResult;
		wxButton* m_buttonClear;
		wxButton* m_buttonCopyResultFormula;
		wxButton* m_buttonCancel;
		wxButton* m_buttonOK;

		// Virtual event handlers, override them in your derived class
		virtual void OnItemActivated( wxDataViewEvent& event ) { event.Skip(); }
		virtual void OnClearFormula( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCopyResultToFormula( wxCommandEvent& event ) { event.Skip(); }


	public:

		GUIDialogFormulaEditor( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Formula Editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 650,450 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );

		~GUIDialogFormulaEditor();

};

///////////////////////////////////////////////////////////////////////////////
/// Class GUIFrameSetupUnits
///////////////////////////////////////////////////////////////////////////////
class GUIFrameSetupUnits : public wxFrame
{
	private:

	protected:
		wxStaticText* m_staticText57;
		wxChoice* m_choiceUnitLength;
		wxStaticText* m_staticText110;
		wxChoice* m_choiceUnitSmallDistance;
		wxStaticText* m_staticText112;
		wxChoice* m_choiceUnitAngle;
		wxButton* m_buttonClose;
		wxStaticText* m_staticText64;

		// Virtual event handlers, override them in your derived class
		virtual void OnCloseX( wxCloseEvent& event ) = 0;
		virtual void OnChangeUnit( wxCommandEvent& event ) = 0;
		virtual void OnClose( wxCommandEvent& event ) = 0;


	public:

		GUIFrameSetupUnits( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Setup display units"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxCLOSE_BOX|wxSTAY_ON_TOP|wxSYSTEM_MENU|wxTAB_TRAVERSAL );

		~GUIFrameSetupUnits();

};

///////////////////////////////////////////////////////////////////////////////
/// Class GUIFrameSetupBackgroundImages
///////////////////////////////////////////////////////////////////////////////
class GUIFrameSetupBackgroundImages : public wxFrame
{
	private:

	protected:
		wxListCtrl* m_listCtrl;
		wxButton* m_buttonAdd;
		wxButton* m_buttonRemove;
		wxStaticText* m_staticTextFilename;
		wxFilePickerCtrl* m_filePicker;
		wxStaticText* m_staticTextDirection;
		wxCheckBox* m_checkBoxFront;
		wxCheckBox* m_checkBoxBack;
		wxCheckBox* m_checkBoxLeft;
		wxCheckBox* m_checkBoxRight;
		wxCheckBox* m_checkBoxTop;
		wxCheckBox* m_checkBoxBottom;
		wxStaticText* m_staticText46;
		ExtendedTextCtrl* m_textCtrlScale;
		wxSpinButton* m_spinBtnScale;
		wxStaticText* m_staticTextOffsHor;
		ExtendedTextCtrl* m_textCtrlOffsHor;
		wxSpinButton* m_spinBtnOffsHor;
		wxStaticText* m_staticTextOffsVert;
		ExtendedTextCtrl* m_textCtrlOffsVert;
		wxSpinButton* m_spinBtnOffsVert;
		wxStaticText* m_staticTextRotation;
		wxSlider* m_sliderRotation;
		wxButton* m_buttonClose;

		// Virtual event handlers, override them in your derived class
		virtual void OnAddUpdate( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRemove( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTextEnter( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSpin( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnRightDown( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnScroll( wxScrollEvent& event ) { event.Skip(); }


	public:

		GUIFrameSetupBackgroundImages( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Setup background images"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 582,561 ), long style = wxDEFAULT_FRAME_STYLE|wxRESIZE_BORDER|wxSTAY_ON_TOP|wxTAB_TRAVERSAL );

		~GUIFrameSetupBackgroundImages();

};

///////////////////////////////////////////////////////////////////////////////
/// Class GUISetupPaths
///////////////////////////////////////////////////////////////////////////////
class GUISetupPaths : public wxDialog
{
	private:

	protected:
		wxStaticText* m_staticTextMeasurementsDirectory;
		wxDirPickerCtrl* m_dirPickerMeasurementsDirectory;
		wxStaticText* m_staticTextShoeDesignDirectory;
		wxDirPickerCtrl* m_dirPickerShoeDesignDirectory;
		wxStaticText* m_staticTextLastDirectory;
		wxDirPickerCtrl* m_dirPickerLastDirectory;
		wxStaticText* m_staticTextOutputDirectory;
		wxDirPickerCtrl* m_dirPickerOutputDirectory;
		wxButton* m_buttonClose;

		// Virtual event handlers, override them in your derived class
		virtual void OnClose( wxCommandEvent& event ) { event.Skip(); }


	public:

		GUISetupPaths( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Path setup"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 547,342 ), long style = wxDEFAULT_DIALOG_STYLE );

		~GUISetupPaths();

};

///////////////////////////////////////////////////////////////////////////////
/// Class GUIDialogMidiSetup
///////////////////////////////////////////////////////////////////////////////
class GUIDialogMidiSetup : public wxDialog
{
	private:

	protected:
		wxStaticText* m_staticText;
		wxChoice* m_choice;
		wxButton* m_buttonConnectDisconnect;
		wxButton* m_buttonClose;

		// Virtual event handlers, override them in your derived class
		virtual void OnConnectDisconnect( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClose( wxCommandEvent& event ) { event.Skip(); }


	public:

		GUIDialogMidiSetup( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Midi Controller"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 321,175 ), long style = wxDEFAULT_DIALOG_STYLE );

		~GUIDialogMidiSetup();

};

///////////////////////////////////////////////////////////////////////////////
/// Class GUIDialogAnisotropy
///////////////////////////////////////////////////////////////////////////////
class GUIDialogAnisotropy : public wxDialog
{
	private:

	protected:
		wxButton* m_buttonAdd;
		wxButton* m_buttonRemove;
		PanelAnisotropy* m_panelAnisotropy;

		// Virtual event handlers, override them in your derived class
		virtual void OnAddPoint( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRemovePoint( wxCommandEvent& event ) { event.Skip(); }


	public:

		GUIDialogAnisotropy( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Editor Anisotropy"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 670,719 ), long style = wxDEFAULT_DIALOG_STYLE );

		~GUIDialogAnisotropy();

};

