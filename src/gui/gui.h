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
#include <wx/radiobut.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/statbox.h>
#include <wx/statbmp.h>
#include <wx/panel.h>
#include <wx/filepicker.h>
#include <wx/choicebk.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/dataview.h>
#include <wx/stc/stc.h>
#include <wx/treelist.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/listbox.h>
#include <wx/notebook.h>
#include "Canvas3D.h"
#include "CanvasInsole.h"
#include "CanvasPattern.h"
#include "CanvasFlattening.h"
#include "CanvasGraph.h"
#include "CanvasTestStitch.h"
#include "CanvasSupport.h"
#include "CanvasWalkcycle.h"
#include <wx/slider.h>
#include <wx/wrapsizer.h>
#include <wx/splitter.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/grid.h>
#include "CanvasFootModel.h"
#include <wx/dialog.h>
#include <wx/listctrl.h>
#include <wx/spinbutt.h>
#include "CanvasAnisotropy.h"

///////////////////////////////////////////////////////////////////////////

#define ID_MEASUREMENTS_SETUP 1000
#define ID_MEASUREMENTS 1001
#define ID_SCAN 1002
#define ID_ACTIVETOINACTIVE 1003
#define ID_RIGHTTOLEFT 1004
#define ID_LEFTTORIGHT 1005
#define ID_CONSTRUCTION 1006
#define ID_BONES 1007
#define ID_LAST 1008
#define ID_BONES_EDIT 1009
#define ID_CONSTRUCTION_EXPERIMENTAL 1010
#define ID_CONSTRUCTION_WELDED 1011
#define ID_CONSTRUCTION_CEMENTED 1012
#define ID_CONSTRUCTION_MOLDED 1013
#define ID_CONSTRUCTION_DUTCH 1014
#define ID_CONSTRUCTION_GETA 1015
#define ID_PATTERN 1016
#define ID_PATTERN_SAVE 1017
#define ID_FLATTENING 1018
#define ID_FABRICS 1019
#define ID_INSOLE 1020
#define ID_HEEL 1021
#define ID_CUTAWAY 1022
#define ID_ZIP 1023
#define ID_STEREO3D 1024
#define ID_LEFT 1025
#define ID_RIGHT 1026
#define ID_SKIN 1027
#define ID_LEG 1028
#define ID_UPPER 1029
#define ID_FLOOR 1030
#define ID_COORDINATESYSTEM 1031
#define ID_BACKGROUND 1032
#define ID_BACKGROUND_SETUP 1033
#define ID_CALCULATOR 1034
#define ID_BOTH 1035
#define ID_MEASUREMENTSOURCE 1036
#define ID_MEASUREMENT_FOOTLENGTH 1037
#define ID_MEASUREMENT_BALLWIDTH 1038
#define ID_MEASUREMENT_BIGTOEGIRTH 1039
#define ID_MEASUREMENT_LITTLETOEGIRTH 1040
#define ID_MEASUREMENT_WAISTGIRTH 1041
#define ID_MEASUREMENT_HEELGIRTH 1042
#define ID_MEASUREMENT_HEELWIDTH 1043
#define ID_MEASUREMENT_LEGLENGTHDIFFERENCE 1044
#define ID_IMAGEFOOT 1045
#define ID_MEASUREMENT_BELOWCROTCHGIRTH 1046
#define ID_MEASUREMENT_BELOWCROTCHLEVEL 1047
#define ID_MEASUREMENT_MIDDLEOFTHIGHGIRTH 1048
#define ID_MEASUREMENT_MIDDLEOFTHIGHLEVEL 1049
#define ID_MEASUREMENT_ABOVEKNEEGIRTH 1050
#define ID_MEASUREMENT_ABOVEKNEELEVEL 1051
#define ID_MEASUREMENT_OVERKNEECAPGIRTH 1052
#define ID_MEASUREMENT_OVERKNEECAPLEVEL 1053
#define ID_MEASUREMENT_BELOWKNEEGIRTH 1054
#define ID_MEASUREMENT_BELOWKNEELEVEL 1055
#define ID_MEASUREMENT_MIDDLEOFSHANKGIRTH 1056
#define ID_MEASUREMENT_MIDDLEOFSHANKLEVEL 1057
#define ID_MEASUREMENT_ABOVEANKLEGIRTH 1058
#define ID_MEASUREMENT_ABOVEANKLELEVEL 1059
#define ID_MEASUREMENT_OVERANKLEBONEGIRTH 1060
#define ID_MEASUREMENT_OVERANKLEBONELEVEL 1061
#define ID_LASTCONSTRUCTIONTYPE 1062
#define ID_BIGTOEANGLE 1063
#define ID_LITTLETOEANGLE 1064
#define ID_BALLMEASUREMENTANGLE 1065
#define ID_HEELDIRECTIONANGLE 1066
#define ID_TIPSHARPNESS 1067
#define ID_MEASUREMENT_ANGLEMIXING 1068
#define ID_EXTRALENGTH 1069
#define ID_FOOTCOMPRESSION 1070
#define ID_HEELREORIENT 1071
#define ID_LASTMODIFY 1072
#define ID_LASTREORIENT 1073
#define ID_HEELCONSTRUCTIONTYPE 1074
#define ID_PRESETSHOETYPE 1075
#define ID_HEELHEIGHT 1076
#define ID_BALLHEIGHT 1077
#define ID_HEELPITCH 1078
#define ID_TOESPRING 1079
#define ID_PRESETSHOEHEIGHT 1080
#define ID_UPPERLEVEL 1081
#define ID_GENERATOR 1082
#define ID_WELTSIZE 1083
#define ID_THICKNESS 1084
#define ID_BRIDGE 1085
#define ID_SUPPORTHEELRADIUS 1086
#define ID_SUPPORTHEELOFFSET 1087
#define ID_SUPPORTTOERADIUS 1088
#define ID_SUPPORTTOEOFFSET 1089
#define ID_PRESETHEEL 1090
#define ID_PRESETHEELCODE 1091
#define ID_PRESETHEELVARIANT 1092
#define ID_HEELCODE 1093
#define ID_HEELVARIANT 1094
#define ID_SELECTED 1095
#define ID_LINE 1096
#define ID_PATCH 1097
#define ID_STITCHING 1098
#define ID_PADDING 1099
#define ID_LACES 1100
#define ID_ACCESSORY 1101
#define ID_MAINVIEW 1102
#define ID_ANKLELOCK 1103
#define ID_SPEED 1104
#define ID_DISPLAY 1105
#define ID_GRIDDIAMETER 1106
#define ID_GRIDLENGTH 1107
#define ID_GRIDSKIN 1108
#define ID_FORMULA 1109
#define ID_SELECTION 1110
#define ID_IMAGE 1111
#define ID_FRONT 1112
#define ID_BACK 1113
#define ID_TOP 1114
#define ID_BOTTOM 1115
#define ID_SCALE 1116
#define ID_OFFSETHORIZONTAL 1117
#define ID_OFFSETVERTICAL 1118
#define ID_ROTATION 1119
#define ID_POINT 1120
#define ID_CODE 1121
#define ID_XMIN 1122
#define ID_XMAX 1123
#define ID_YMIN 1124
#define ID_YMAX 1125
#define ID_ZMIN 1126
#define ID_ZMAX 1127
#define ID_AUTORUN 1128
#define ID_MAXSTEPS 1129
#define ID_RUN 1130
#define ID_STOP 1131
#define ID_STEPASM 1132
#define ID_STEPINSTRUCTION 1133

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
		wxMenu* m_menuLast;
		wxMenu* m_menuLastModel;
		wxMenu* m_menuBoneModel;
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
		wxRadioButton* m_radioBtnEditLeft;
		wxRadioButton* m_radioBtnEditBoth;
		wxRadioButton* m_radioBtnEditRight;
		wxChoicebook* m_choicebookMeasurementSource;
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
		wxFilePickerCtrl* m_filePickerScan;
		wxPanel* m_panelPageLeg;
		wxRadioButton* m_radioBtnEditLegLeft;
		wxRadioButton* m_radioBtnEditLegBoth;
		wxRadioButton* m_radioBtnEditLegRight;
		wxStaticBitmap* m_bitmapLeg;
		wxStaticText* m_staticTextGirth;
		wxStaticText* m_staticTextLevel;
		ExtendedTextCtrl* m_textCtrlBelowCrotchGirth;
		ExtendedTextCtrl* m_textCtrlBelowCrotchLevel;
		ExtendedTextCtrl* m_textCtrlMiddleOfThighGirth;
		ExtendedTextCtrl* m_textCtrlMiddleOfThighLevel;
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
		wxPanel* m_panelPageLast;
		wxChoicebook* m_choicebookLastConstructionType;
		wxPanel* m_panelLastConstruct;
		wxStaticText* m_staticTextBigToeAngle;
		ExtendedTextCtrl* m_textCtrlBigToeAngle;
		wxStaticText* m_staticTextLittleToeAngle;
		ExtendedTextCtrl* m_textCtrlLittleToeAngle;
		wxStaticText* m_staticTextBallMeasurementAngle;
		ExtendedTextCtrl* m_textCtrlBallMeasurementAngle;
		wxStaticText* m_staticTextHeelDirectionAngle;
		ExtendedTextCtrl* m_textCtrlHeelDirectionAngle;
		wxStaticText* m_staticTextTipSharpness;
		ExtendedTextCtrl* m_textCtrlTipSharpness;
		wxStaticText* m_staticTextAngleMixing;
		ExtendedTextCtrl* m_textCtrlAngleMixing;
		wxStaticText* m_staticTextExtraLength;
		ExtendedTextCtrl* m_textCtrlExtraLength;
		wxStaticText* m_staticTextFootCompression;
		ExtendedTextCtrl* m_textCtrlFootCompression;
		wxPanel* m_panelLastBoneBased;
		wxStaticText* m_staticTextBondeModel;
		wxFilePickerCtrl* m_filePickerBoneModel;
		wxButton* m_buttonEditBoneModel;
		wxPanel* m_panelLastLoadFromFile;
		wxStaticText* m_staticTextLastModel;
		wxFilePickerCtrl* m_filePickerLast;
		wxCheckBox* m_checkBoxLastModify;
		wxCheckBox* m_checkBoxLastReorient;
		wxChoicebook* m_choicebookHeelConstructionType;
		wxPanel* m_panelHeelFromCode;
		wxStaticText* m_staticTextHeelFromCode;
		wxPanel* m_panelHeelFromFile;
		wxStaticText* m_staticTextHeel;
		wxFilePickerCtrl* m_filePickerHeel;
		wxCheckBox* m_checkBoxHeelReorient;
		wxPanel* m_panelPageShoe;
		wxChoice* m_choicePresetShoeType;
		wxStaticText* m_staticTextHeelHeight;
		ExtendedTextCtrl* m_textCtrlHeelHeight;
		wxStaticText* m_staticTextBallHeight;
		ExtendedTextCtrl* m_textCtrlBallHeight;
		wxStaticText* m_staticTextHeelPitch;
		ExtendedTextCtrl* m_textCtrlHeelPitch;
		wxStaticText* m_staticTextToeSpring;
		ExtendedTextCtrl* m_textCtrlToeSpring;
		wxChoice* m_choicePresetShoeStyle;
		wxStaticText* m_staticTextUpperLevel;
		ExtendedTextCtrl* m_textCtrlUpperLevel;
		wxStaticText* m_staticTextConstruction;
		wxChoicebook* m_choicebookConstruction;
		wxPanel* m_panelExperimental;
		wxPanel* m_panelWelted;
		wxStaticText* m_staticTextWeltSize;
		ExtendedTextCtrl* m_textCtrlWeltSize;
		wxPanel* m_panelCemented;
		wxPanel* m_panelMolded;
		wxPanel* m_panelClog;
		wxStaticText* m_staticTextThickness;
		ExtendedTextCtrl* m_textCtrlThickness;
		wxPanel* m_panelGeta;
		wxStaticText* m_staticTextBridge;
		wxButton* m_buttonAddBridge;
		wxButton* m_buttonDeleteBridge;
		wxDataViewListCtrl* m_dataViewListCtrlBridges;
		wxDataViewColumn* m_dataViewListColumnBridgePosition;
		wxDataViewColumn* m_dataViewListColumnBridgeWidth;
		wxPanel* m_panelPageHeel;
		wxStaticText* m_staticTextSupportHeelRadius;
		ExtendedTextCtrl* m_textCtrlSupportHeelRadius;
		wxStaticText* m_staticTextSupportHeelOffset;
		ExtendedTextCtrl* m_textCtrlSupportHeelOffset;
		wxStaticText* m_staticTextSupportToeRadius;
		ExtendedTextCtrl* m_textCtrlSupportToeRadius;
		wxStaticText* m_staticTextSupportToeOffset;
		ExtendedTextCtrl* m_textCtrlSupportToeOffset;
		wxStaticText* m_staticTextHeelPreset;
		wxChoice* m_choicePresetHeel;
		wxStaticText* m_staticTextHeelCode;
		wxStaticText* m_staticTextHeelVariant;
		wxChoice* m_choicePresetHeelCode;
		wxChoice* m_choicePresetHeelVariant;
		wxStyledTextCtrl* m_editorCode;
		wxDataViewListCtrl* m_dataViewListCtrlVariant;
		wxDataViewColumn* m_dataViewListColumnVariantName;
		wxDataViewColumn* m_dataViewListColumnVariantValue;
		wxPanel* m_panelPagePattern;
		wxTreeListCtrl* m_treeListCtrlPattern;
		wxButton* m_buttonElementCopy;
		wxButton* m_buttonElementDelete;
		wxButton* m_buttonOnPatternAddLine;
		wxButton* m_buttonOnPatternAddPatch;
		wxButton* m_buttonOnPatternAddStitching;
		wxButton* m_buttonOnPatternAddPadding;
		wxButton* m_buttonOnPatternAddCoordinateSystem;
		wxButton* m_buttonOnPatternAddLaces;
		wxButton* m_buttonOnPatternAddAccessory;
		wxPropertyGrid* m_propertyGridPatternElement;
		wxPGProperty* m_propertyGridItemName;
		wxPGProperty* m_propertyGridItemVisible;
		wxPGProperty* m_propertyGridItemFabric;
		wxListBox* m_listBoxFabric;
		wxPanel* m_panelCanvas;
		wxNotebook* m_notebookCanvas;
		wxPanel* m_panelCanvas3D;
		Canvas3D* m_canvas3D;
		wxPanel* m_panelCanvasInsole;
		CanvasInsole* m_canvasInsole;
		wxPanel* m_panelCanvasPattern;
		CanvasPattern* m_canvasPattern;
		wxPanel* m_panelFlattening;
		CanvasFlattening* m_canvasFlattening;
		CanvasGraph* m_canvasGraph;
		wxPanel* m_panelCanvasTestStitch;
		CanvasTestStitch* m_canvasTestStitch;
		wxPanel* m_panelCanvasAnalysis;
		CanvasSupport* m_canvasSupport;
		CanvasWalkcycle* m_canvasCycle;
		wxCheckBox* m_checkBoxLockAnkle;
		wxSlider* m_sliderSpeed;
		ExtendedTextCtrl* m_textCtrlSpeed;
		wxChoice* m_choiceDisplay;
		wxStatusBar* m_statusBar;

		// Virtual event handlers, override them in your derived class
		virtual void OnClose( wxCloseEvent& event ) = 0;
		virtual void OnMeasurementsQuickSetup( wxCommandEvent& event ) = 0;
		virtual void OnModelChanged( wxCommandEvent& event ) = 0;
		virtual void OnMeasurementsCopy( wxCommandEvent& event ) = 0;
		virtual void OnObjectLoad( wxCommandEvent& event ) = 0;
		virtual void OnObjectEdit( wxCommandEvent& event ) = 0;
		virtual void OnConstructionChanged( wxCommandEvent& event ) = 0;
		virtual void OnObjectSave( wxCommandEvent& event ) = 0;
		virtual void OnViewChanged( wxCommandEvent& event ) = 0;
		virtual void OnToggleStereo3D( wxCommandEvent& event ) = 0;
		virtual void OnBackgroundImagesSetup( wxCommandEvent& event ) = 0;
		virtual void OnCalculator( wxCommandEvent& event ) = 0;
		virtual void OnMouseWheel( wxMouseEvent& event ) = 0;
		virtual void OnRadioButton( wxCommandEvent& event ) = 0;
		virtual void OnPageChanged( wxChoicebookEvent& event ) = 0;
		virtual void OnKillFocus( wxFocusEvent& event ) = 0;
		virtual void OnSetFocus( wxFocusEvent& event ) = 0;
		virtual void OnTextEnter( wxCommandEvent& event ) = 0;
		virtual void OnSize( wxSizeEvent& event ) = 0;
		virtual void OnFileChanged( wxFileDirPickerEvent& event ) = 0;
		virtual void OnCheckBox( wxCommandEvent& event ) = 0;
		virtual void OnChoice( wxCommandEvent& event ) = 0;
		virtual void OnButtonAdd( wxCommandEvent& event ) = 0;
		virtual void OnButtonDelete( wxCommandEvent& event ) = 0;
		virtual void OnListCtrlOnSelectionChanged( wxDataViewEvent& event ) = 0;
		virtual void OnChar( wxKeyEvent& event ) = 0;
		virtual void OnDataViewListCtrlItemEditingDone( wxDataViewEvent& event ) = 0;
		virtual void OnPatternSelect( wxTreeListEvent& event ) = 0;
		virtual void OnButtonCopy( wxCommandEvent& event ) = 0;
		virtual void OnPatternSelectFabric( wxCommandEvent& event ) = 0;
		virtual void OnNotebookPageChanged( wxNotebookEvent& event ) = 0;
		virtual void On3DSelect( wxMouseEvent& event ) = 0;
		virtual void OnScroll( wxScrollEvent& event ) = 0;


	public:

		GUIFrameMain(wxDocument* doc, wxView* view, wxDocParentFrame* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Open Shoe Designer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1193,800 ), long style = wxDEFAULT_FRAME_STYLE|wxRESIZE_BORDER|wxTAB_TRAVERSAL );

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
		wxMenuBar* m_menubar;
		wxMenu* m_menuFile;
		wxMenu* m_menuHelp;
		wxNotebook* m_notebook;
		wxPanel* m_panelBone;
		wxPanel* m_panelBoneDiameter;
		wxGrid* m_gridBoneDiameter;
		wxGrid* m_gridBoneLength;
		wxPanel* m_panelSkinThickness;
		wxGrid* m_gridSkinThickness;

		// Virtual event handlers, override them in your derived class
		virtual void OnReset( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLoad( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSave( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSaveAs( wxCommandEvent& event ) { event.Skip(); }
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

		GUIFrameDebugParser( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Test Parser and Units"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxCLOSE_BOX|wxFRAME_FLOAT_ON_PARENT|wxFRAME_NO_TASKBAR|wxRESIZE_BORDER|wxSTAY_ON_TOP|wxSYSTEM_MENU|wxTAB_TRAVERSAL );

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
		ExtendedTextCtrl* m_textCtrlResult;
		wxButton* m_buttonClear;
		wxButton* m_buttonCopyResultFormula;
		wxButton* m_buttonCancel;
		wxButton* m_buttonOK;

		// Virtual event handlers, override them in your derived class
		virtual void OnItemActivated( wxDataViewEvent& event ) { event.Skip(); }
		virtual void OnTextEnter( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClear( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonCopy( wxCommandEvent& event ) { event.Skip(); }


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
		virtual void OnButtonAdd( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonRemove( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCheckBox( wxCommandEvent& event ) { event.Skip(); }
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
		wxStaticText* m_staticTextMeasurements;
		wxDirPickerCtrl* m_dirPickerMeasurementsDirectory;
		wxStaticText* m_staticTextPresetDirectory;
		wxDirPickerCtrl* m_dirPickerPresetDirectory;
		wxStaticText* m_staticTextLastDirectory;
		wxDirPickerCtrl* m_dirPickerLastDirectory;
		wxStaticText* m_staticTextFabricDirectory;
		wxDirPickerCtrl* m_dirPickerFabricDirectory;
		wxStaticText* m_staticTextOutputDirectory;
		wxDirPickerCtrl* m_dirPickerOutputDirectory;
		wxButton* m_buttonClose;

		// Virtual event handlers, override them in your derived class
		virtual void OnDirChanged( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void OnClose( wxCommandEvent& event ) { event.Skip(); }


	public:

		GUISetupPaths( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Path setup"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 600,423 ), long style = wxDEFAULT_DIALOG_STYLE );

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

		GUIDialogMidiSetup( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Midi Controller"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 321,-1 ), long style = wxDEFAULT_DIALOG_STYLE );

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
		CanvasAnisotropy* m_canvasAnisotropy;

		// Virtual event handlers, override them in your derived class
		virtual void OnButtonAdd( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonRemove( wxCommandEvent& event ) { event.Skip(); }


	public:

		GUIDialogAnisotropy( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Editor Anisotropy"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 670,719 ), long style = wxDEFAULT_DIALOG_STYLE );

		~GUIDialogAnisotropy();

};

///////////////////////////////////////////////////////////////////////////////
/// Class GUIFrameCalculator
///////////////////////////////////////////////////////////////////////////////
class GUIFrameCalculator : public wxFrame
{
	private:

	protected:
		wxSplitterWindow* m_splitterVertical;
		wxPanel* m_panelLeft;
		wxSplitterWindow* m_splitterHorizontal;
		wxPanel* m_panelTop;
		wxStyledTextCtrl* m_textCtrlCode;
		wxPanel* m_panelBottom;
		wxNotebook* m_notebook;
		wxPanel* m_panelX;
		CanvasGraph* m_canvasGraph;
		wxStaticText* m_staticTextXMin;
		ExtendedTextCtrl* m_textCtrlXMin;
		wxStaticText* m_staticTextXMax;
		ExtendedTextCtrl* m_textCtrlXMax;
		wxPanel* m_panelXY;
		Canvas3D* m_canvasXY;
		wxStaticText* m_staticTextXMin2;
		ExtendedTextCtrl* m_textCtrlXMin2;
		wxStaticText* m_staticTextXMax2;
		ExtendedTextCtrl* m_textCtrlXMax2;
		wxStaticText* m_staticTextYMin2;
		ExtendedTextCtrl* m_textCtrlYMin2;
		wxStaticText* m_staticTextYMax2;
		ExtendedTextCtrl* m_textCtrlYMax2;
		wxPanel* m_panelXYZ;
		Canvas3D* m_canvasXYZ;
		wxStaticText* m_staticTextXMin3;
		ExtendedTextCtrl* m_textCtrlXMin3;
		wxStaticText* m_staticTextXMax3;
		ExtendedTextCtrl* m_textCtrlXMax3;
		wxStaticText* m_staticTextYMin3;
		ExtendedTextCtrl* m_textCtrlYMin3;
		wxStaticText* m_staticTextYMax3;
		ExtendedTextCtrl* m_textCtrlYMax3;
		wxStaticText* m_staticTextZMin3;
		ExtendedTextCtrl* m_textCtrlZMin3;
		wxStaticText* m_staticTextZMax3;
		ExtendedTextCtrl* m_textCtrlZMax3;
		wxPanel* m_panelRight;
		wxStaticText* m_staticTextVariables;
		wxPropertyGrid* m_propertyGridVariables;
		wxStaticText* m_staticTextStack;
		wxStyledTextCtrl* m_textCtrlStack;
		wxStaticText* m_staticTextAsm;
		wxStyledTextCtrl* m_textCtrlAsm;
		wxCheckBox* m_checkBoxAutorun;
		wxStaticText* m_staticTextMaxSteps;
		ExtendedTextCtrl* m_textCtrlMaxSteps;
		wxButton* m_buttonRun;
		wxButton* m_buttonReset;
		wxButton* m_buttonStepAsm;
		wxButton* m_buttonStepExpr;

		// Virtual event handlers, override them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnNotebookPageChanged( wxNotebookEvent& event ) { event.Skip(); }
		virtual void OnNotebookPageChanging( wxNotebookEvent& event ) { event.Skip(); }
		virtual void OnKillFocus( wxFocusEvent& event ) { event.Skip(); }
		virtual void OnTextEnter( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPropertyGridChanged( wxPropertyGridEvent& event ) { event.Skip(); }
		virtual void OnPropertyGridChanging( wxPropertyGridEvent& event ) { event.Skip(); }
		virtual void OnCheckBox( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButton( wxCommandEvent& event ) { event.Skip(); }


	public:

		GUIFrameCalculator( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Calculator"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1396,1103 ), long style = wxDEFAULT_FRAME_STYLE|wxRESIZE_BORDER|wxTAB_TRAVERSAL );

		~GUIFrameCalculator();

		void m_splitterVerticalOnIdle( wxIdleEvent& )
		{
			m_splitterVertical->SetSashPosition( 1040 );
			m_splitterVertical->Disconnect( wxEVT_IDLE, wxIdleEventHandler( GUIFrameCalculator::m_splitterVerticalOnIdle ), NULL, this );
		}

		void m_splitterHorizontalOnIdle( wxIdleEvent& )
		{
			m_splitterHorizontal->SetSashPosition( 344 );
			m_splitterHorizontal->Disconnect( wxEVT_IDLE, wxIdleEventHandler( GUIFrameCalculator::m_splitterHorizontalOnIdle ), NULL, this );
		}

};

