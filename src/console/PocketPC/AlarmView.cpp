// AlarmView.cpp : implementation file
//

#include "stdafx.h"
#include "nxpc.h"
#include "AlarmView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TEXT_LEFT_MARGIN	40


//
// Alarm comparision function
//

static int CALLBACK CompareListItems(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
   NXC_ALARM *pAlarm1, *pAlarm2;
   NXC_OBJECT *pObject1, *pObject2;
   TCHAR szName1[MAX_OBJECT_NAME], szName2[MAX_OBJECT_NAME];
   int iResult;

   pAlarm1 = ((CAlarmView *)lParamSort)->FindAlarmInList(lParam1);
   pAlarm2 = ((CAlarmView *)lParamSort)->FindAlarmInList(lParam2);
   if ((pAlarm1 == NULL) || (pAlarm2 == NULL))
      return 0;

   switch(((CAlarmView *)lParamSort)->SortMode())
   {
      case SORT_BY_SEVERITY:
         iResult = COMPARE_NUMBERS(pAlarm1->nCurrentSeverity, pAlarm2->nCurrentSeverity);
         break;
      case SORT_BY_SOURCE:
         pObject1 = NXCFindObjectById(g_hSession, pAlarm1->dwSourceObject);
         pObject2 = NXCFindObjectById(g_hSession, pAlarm2->dwSourceObject);

         if (pObject1 == NULL)
            _tcscpy(szName1, _T("<unknown>"));
         else
            _tcscpy(szName1, pObject1->szName);
         
         if (pObject2 == NULL)
            _tcscpy(szName2, _T("<unknown>"));
         else
            _tcscpy(szName2, pObject2->szName);

         iResult = _tcsicmp(szName1, szName2);
         break;
      case SORT_BY_MESSAGE:
         iResult = _tcsicmp(pAlarm1->szMessage, pAlarm2->szMessage);
         break;
      case SORT_BY_TIMESTAMP:
         iResult = COMPARE_NUMBERS(pAlarm1->dwLastChangeTime, pAlarm2->dwLastChangeTime);
         break;
      default:
         iResult = 0;
         break;
   }
   return (((CAlarmView *)lParamSort)->SortDir() == 0) ? iResult : -iResult;
}


/////////////////////////////////////////////////////////////////////////////
// CAlarmView

CAlarmView::CAlarmView()
{
   m_pImageList = NULL;
   memset(m_iNumAlarms, 0, sizeof(int) * 5);
   m_iSortMode = SORT_BY_SEVERITY;
   m_iSortDir = 1;
   m_dwNumAlarms = 0;
   m_pAlarmList = NULL;
	m_rgbLine1 = RGB(255, 255, 255);
	m_rgbLine2 = RGB(255, 255, 192);
	m_rgbText = RGB(0, 0, 0);
}

CAlarmView::~CAlarmView()
{
   delete m_pImageList;
}


BEGIN_MESSAGE_MAP(CAlarmView, CWnd)
	//{{AFX_MSG_MAP(CAlarmView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DRAWITEM()
	ON_COMMAND(ID_VIEW_REFRESH, OnViewRefresh)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_ALARM_SORTBY_ASCENDING, OnAlarmSortbyAscending)
	ON_COMMAND(ID_ALARM_SORTBY_DESCENDING, OnAlarmSortbyDescending)
	ON_COMMAND(ID_ALARM_SORTBY_SEVERITY, OnAlarmSortbySeverity)
	ON_COMMAND(ID_ALARM_SORTBY_SOURCE, OnAlarmSortbySource)
	ON_COMMAND(ID_ALARM_SORTBY_MESSAGETEXT, OnAlarmSortbyMessagetext)
	ON_COMMAND(ID_ALARM_SORTBY_TIMESTAMP, OnAlarmSortbyTimestamp)
	ON_COMMAND(ID_ALARM_ACKNOWLEDGE, OnAlarmAcknowledge)
	ON_COMMAND(ID_ALARM_DELETE, OnAlarmDelete)
	ON_COMMAND(ID_ALARM_TERMINATE, OnAlarmTerminate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAlarmView message handlers


//
// WM_CREATE message handler
//

int CAlarmView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
   RECT rect;

	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

   // Create font for alarm list
   m_fontSmall.CreateFont(-MulDiv(7, GetDeviceCaps(GetDC()->m_hDC, LOGPIXELSY), 72),
                          0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
                          OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                          VARIABLE_PITCH | FF_DONTCARE, L"Microsoft Sans Serif");
   m_fontLarge.CreateFont(-MulDiv(14, GetDeviceCaps(GetDC()->m_hDC, LOGPIXELSY), 72),
                          0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
                          OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                          VARIABLE_PITCH | FF_DONTCARE, L"Microsoft Sans Serif");

   GetClientRect(&rect);
   m_wndListCtrl.Create(WS_CHILD | WS_VISIBLE | LVS_REPORT | 
                        LVS_NOCOLUMNHEADER | LVS_OWNERDRAWFIXED, rect, this, ID_LIST_CTRL);
   m_wndListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);
   m_wndListCtrl.SetFont(&m_fontLarge, FALSE);
	
   // Create image list
   m_pImageList = CreateEventImageList();
	m_pImageList->Add(theApp.LoadIcon(IDI_ACK));
	m_pImageList->Add(theApp.LoadIcon(IDI_OUTSTANDING));
   m_wndListCtrl.SetImageList(m_pImageList, LVSIL_SMALL);

   // Setup columns
	m_wndListCtrl.GetClientRect(&rect);
	m_cx = rect.right - GetSystemMetrics(SM_CXVSCROLL);
   m_wndListCtrl.InsertColumn(0, _T("Severity"), LVCFMT_LEFT, 1);
   m_wndListCtrl.InsertColumn(1, _T("Source"), LVCFMT_LEFT, 1);
   m_wndListCtrl.InsertColumn(2, _T("Message"), LVCFMT_LEFT, 1);
   m_wndListCtrl.InsertColumn(3, _T("Time Stamp"), LVCFMT_LEFT, 1);
   m_wndListCtrl.InsertColumn(4, _T("Ack"), LVCFMT_CENTER, m_cx);

	return 0;
}


//
// WM_SIZE message handler
//

void CAlarmView::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

   m_wndListCtrl.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
}


//
// WM_DRAWITEM message handler
//

void CAlarmView::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
   if (nIDCtl == ID_LIST_CTRL)
   {
      CDC *pdc;

      pdc = CDC::FromHandle(lpDrawItemStruct->hDC);
      DrawListItem(*pdc, lpDrawItemStruct->rcItem, lpDrawItemStruct->itemID,
                   lpDrawItemStruct->itemData);
//      pdc->Detach();
//      delete pdc;
   }
   else
   {
	   CWnd::OnDrawItem(nIDCtl, lpDrawItemStruct);
   }
}


//
// WM_COMMAND::ID_VIEW_REFRESH message handler
//

void CAlarmView::OnViewRefresh() 
{
   DWORD i, dwRetCode;
	RECT rect;

   m_wndListCtrl.DeleteAllItems();
   safe_free(m_pAlarmList);
   m_dwNumAlarms = 0;
   m_pAlarmList = NULL;
	m_wndListCtrl.GetClientRect(&rect);
	m_cx = rect.right - GetSystemMetrics(SM_CXVSCROLL);
   dwRetCode = DoRequestArg4(NXCLoadAllAlarms, g_hSession, (void *)FALSE, 
                             &m_dwNumAlarms, &m_pAlarmList, _T("Loading alarms..."));
   if (dwRetCode == RCC_SUCCESS)
   {
      memset(m_iNumAlarms, 0, sizeof(int) * 5);
      for(i = 0; i < m_dwNumAlarms; i++)
         AddAlarm(&m_pAlarmList[i]);
      m_wndListCtrl.SortItems(CompareListItems, (LPARAM)this);
   }
   else
   {
      theApp.ErrorBox(dwRetCode, _T("Error loading alarm list: %s"));
   }
}


//
// Add new alarm to list
//

void CAlarmView::AddAlarm(NXC_ALARM *pAlarm)
{
   int nItem;
   NXC_OBJECT *pObject;

   pObject = NXCFindObjectById(g_hSession, pAlarm->dwSourceObject);
   nItem = m_wndListCtrl.InsertItem(0x7FFFFFFF,
                                    g_szStatusTextSmall[pAlarm->nCurrentSeverity],
                                    pAlarm->nCurrentSeverity);
   if (nItem != -1)
   {
      m_wndListCtrl.SetItemData(nItem, pAlarm->dwAlarmId);
		UpdateAlarm(nItem, pAlarm);
   }
   m_iNumAlarms[pAlarm->nCurrentSeverity]++;
}


//
// Update alarm entry in list control
//

void CAlarmView::UpdateAlarm(int nItem, NXC_ALARM *pAlarm)
{
   TCHAR szBuffer[64];
   NXC_OBJECT *pObject;
	LVITEM lvi;
	int cx;

   pObject = NXCFindObjectById(g_hSession, pAlarm->dwSourceObject);
	lvi.iItem = nItem;
	lvi.iSubItem = 0;
	lvi.mask = LVIF_TEXT | LVIF_IMAGE;
	lvi.pszText = g_szStatusTextSmall[pAlarm->nCurrentSeverity];
	lvi.iImage = pAlarm->nCurrentSeverity;
	m_wndListCtrl.SetItem(&lvi);
   m_wndListCtrl.SetItemText(nItem, 1, pObject->szName);
   m_wndListCtrl.SetItemText(nItem, 2, pAlarm->szMessage);
   m_wndListCtrl.SetItemText(nItem, 3, FormatTimeStamp(pAlarm->dwLastChangeTime, szBuffer, TS_LONG_DATE_TIME));
   m_wndListCtrl.SetItemText(nItem, 4, (pAlarm->nState == ALARM_STATE_ACKNOWLEDGED) ? _T("X") : _T(""));

	// Update list width
	cx = GetTextWidth(pAlarm->szMessage);
	if (cx + TEXT_LEFT_MARGIN > m_cx)
	{
		m_cx = cx + TEXT_LEFT_MARGIN;
		m_wndListCtrl.SetColumnWidth(4, m_cx);
	}
}


//
// Draw alarm list item
//

void CAlarmView::DrawListItem(CDC &dc, RECT &rcItem, int iItem, UINT nData)
{
   CFont *pOldFont;
   int iHeight;
   TCHAR szBuffer[1024];
   COLORREF rgbOldColor;
   LVITEM item;
   RECT rcText;

   // Get item data
   item.mask = LVIF_IMAGE | LVIF_STATE;
   item.iItem = iItem;
   item.iSubItem = 0;
   item.stateMask = LVIS_FOCUSED | LVIS_SELECTED;
   m_wndListCtrl.GetItem(&item);

   // Draw background
   if (item.state & LVIS_SELECTED)
   {
      dc.FillSolidRect(&rcItem, GetSysColor(COLOR_HIGHLIGHT));
      rgbOldColor = dc.SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
   }
   else
   {
//      dc.FillSolidRect(&rcItem, GetSysColor(COLOR_WINDOW));
//      rgbOldColor = dc.SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
      dc.FillSolidRect(&rcItem, ((iItem & 1) == 0) ? m_rgbLine1 : m_rgbLine2);
      rgbOldColor = dc.SetTextColor(m_rgbText);
   }

   // Draw severity icon
   iHeight = rcItem.bottom - rcItem.top;
   m_pImageList->Draw(&dc, item.iImage, 
                      CPoint(rcItem.left + 2, rcItem.top + 2), ILD_TRANSPARENT);

   // Draw state icon
   m_wndListCtrl.GetItemText(iItem, 4, szBuffer, 256);
   iHeight = rcItem.bottom - rcItem.top;
   m_pImageList->Draw(&dc, (szBuffer[0] == L'X') ? STATUS_CRITICAL + 1 : STATUS_CRITICAL + 2, 
                      CPoint(rcItem.left + 20, rcItem.top + 2), ILD_TRANSPARENT);

   memcpy(&rcText, &rcItem, sizeof(RECT));
   rcText.left += TEXT_LEFT_MARGIN;
   pOldFont = dc.SelectObject(&m_fontSmall);
   m_wndListCtrl.GetItemText(iItem, 1, szBuffer, 256);
   wcscat(szBuffer, L" [");
   m_wndListCtrl.GetItemText(iItem, 0, &szBuffer[wcslen(szBuffer)], 256);
   wcscat(szBuffer, L"]\r\n");
   m_wndListCtrl.GetItemText(iItem, 2, &szBuffer[wcslen(szBuffer)], 256);
   dc.DrawText(szBuffer, -1, &rcText, DT_LEFT | DT_NOPREFIX);

   if (item.state & LVIS_FOCUSED)
      dc.DrawFocusRect(&rcItem);

   // Restore DC to original state
   dc.SelectObject(pOldFont);
   dc.SetTextColor(rgbOldColor);
}


//
// Process alarm updates
//

void CAlarmView::OnAlarmUpdate(DWORD dwCode, NXC_ALARM *pAlarm)
{
   int iItem;
	NXC_ALARM *pCurr;

   iItem = FindAlarmRecord(pAlarm->dwAlarmId);
   switch(dwCode)
   {
      case NX_NOTIFY_NEW_ALARM:
         if ((iItem == -1) && (pAlarm->nState != ALARM_STATE_TERMINATED))
         {
            AddAlarm(pAlarm);
            AddAlarmToList(pAlarm);
            m_wndListCtrl.SortItems(CompareListItems, (LPARAM)this);
         }
         break;
      case NX_NOTIFY_ALARM_CHANGED:
         if (iItem != -1)
         {
				pCurr = FindAlarmInList(pAlarm->dwAlarmId);
				if (pCurr != NULL)
				{
	            m_iNumAlarms[pCurr->nCurrentSeverity]--;
					memcpy(pCurr, pAlarm, sizeof(NXC_ALARM));
					UpdateAlarm(iItem, pAlarm);
	            m_iNumAlarms[pAlarm->nCurrentSeverity]++;
					m_wndListCtrl.SortItems(CompareListItems, (LPARAM)this);
				}
         }
         break;
      case NX_NOTIFY_ALARM_TERMINATED:
      case NX_NOTIFY_ALARM_DELETED:
         if (iItem != -1)
         {
            DeleteAlarmFromList(pAlarm->dwAlarmId);
            m_wndListCtrl.DeleteItem(iItem);
            m_iNumAlarms[pAlarm->nCurrentSeverity]--;
         }
         break;
      default:
         break;
   }
}


//
// WM_CONTEXTMENU message handler
//

void CAlarmView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
   CMenu *pMenu;
   int iNumItems, iItem;
   UINT uFlags;
   CPoint pt;

   pt = point;
   pWnd->ScreenToClient(&pt);
   iItem = m_wndListCtrl.HitTest(pt, &uFlags);
   if ((iItem != -1) && (uFlags & LVHT_ONITEM))
   {
      if (m_wndListCtrl.GetItemState(iItem, LVIS_SELECTED) != LVIS_SELECTED)
         SelectListViewItem(&m_wndListCtrl, iItem);
   }

   pMenu = theApp.GetContextMenu(0);

   // Disable or check some menu items
   iNumItems = m_wndListCtrl.GetSelectedCount();
   pMenu->EnableMenuItem(ID_ALARM_ACKNOWLEDGE, MF_BYCOMMAND | (iNumItems > 0) ? MF_ENABLED : MF_GRAYED);
   pMenu->EnableMenuItem(ID_ALARM_DELETE, MF_BYCOMMAND | (iNumItems > 0) ? MF_ENABLED : MF_GRAYED);
   pMenu->CheckMenuRadioItem(ID_ALARM_SORTBY_SEVERITY, ID_ALARM_SORTBY_TIMESTAMP,
                             m_iSortMode + ID_ALARM_SORTBY_SEVERITY, MF_BYCOMMAND);
   pMenu->CheckMenuRadioItem(ID_ALARM_SORTBY_ASCENDING, ID_ALARM_SORTBY_DESCENDING,
                             m_iSortDir ? ID_ALARM_SORTBY_DESCENDING : ID_ALARM_SORTBY_ASCENDING,
                             MF_BYCOMMAND);

   pMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this, NULL);
}


//
// Find alarm record in list control by alarm id
// Will return record index or -1 if no records exist
//

int CAlarmView::FindAlarmRecord(DWORD dwAlarmId)
{
   LVFINDINFO lvfi;

   lvfi.flags = LVFI_PARAM;
   lvfi.lParam = dwAlarmId;
   return m_wndListCtrl.FindItem(&lvfi);
}


//
// Delete alarm from internal alarms list
//

void CAlarmView::DeleteAlarmFromList(DWORD dwAlarmId)
{
   DWORD i;

   for(i = 0; i < m_dwNumAlarms; i++)
      if (m_pAlarmList[i].dwAlarmId == dwAlarmId)
      {
         m_dwNumAlarms--;
         memmove(&m_pAlarmList[i], &m_pAlarmList[i + 1],
                 sizeof(NXC_ALARM) * (m_dwNumAlarms - i));
         break;
      }
}


//
// Add new alarm to internal list
//

void CAlarmView::AddAlarmToList(NXC_ALARM *pAlarm)
{
   m_pAlarmList = (NXC_ALARM *)realloc(m_pAlarmList, sizeof(NXC_ALARM) * (m_dwNumAlarms + 1));
   memcpy(&m_pAlarmList[m_dwNumAlarms], pAlarm, sizeof(NXC_ALARM));
   m_dwNumAlarms++;
}


//
// Find alarm record in internal list
//

NXC_ALARM *CAlarmView::FindAlarmInList(DWORD dwAlarmId)
{
   DWORD i;

   for(i = 0; i < m_dwNumAlarms; i++)
      if (m_pAlarmList[i].dwAlarmId == dwAlarmId)
         return &m_pAlarmList[i];
   return NULL;
}


//
// Sort mode switchers
//

void CAlarmView::OnAlarmSortbyAscending() 
{
   m_iSortDir = 0;
   m_wndListCtrl.SortItems(CompareListItems, (LPARAM)this);
}

void CAlarmView::OnAlarmSortbyDescending() 
{
   m_iSortDir = 1;
   m_wndListCtrl.SortItems(CompareListItems, (LPARAM)this);
}

void CAlarmView::OnAlarmSortbySeverity() 
{
   m_iSortMode = SORT_BY_SEVERITY;
   m_wndListCtrl.SortItems(CompareListItems, (LPARAM)this);
}

void CAlarmView::OnAlarmSortbySource() 
{
   m_iSortMode = SORT_BY_SOURCE;
   m_wndListCtrl.SortItems(CompareListItems, (LPARAM)this);
}

void CAlarmView::OnAlarmSortbyMessagetext() 
{
   m_iSortMode = SORT_BY_MESSAGE;
   m_wndListCtrl.SortItems(CompareListItems, (LPARAM)this);
}

void CAlarmView::OnAlarmSortbyTimestamp() 
{
   m_iSortMode = SORT_BY_TIMESTAMP;
   m_wndListCtrl.SortItems(CompareListItems, (LPARAM)this);
}


//
// Alarm acknowledgement worker function
//

static DWORD AcknowledgeAlarms(DWORD dwNumAlarms, DWORD *pdwAlarmList)
{
   DWORD i, dwResult = RCC_SUCCESS;

   for(i = 0; (i < dwNumAlarms) && (dwResult == RCC_SUCCESS); i++)
      dwResult = NXCAcknowledgeAlarm(g_hSession, pdwAlarmList[i]);
   return dwResult;
}


//
// WM_COMMAND::ID_ALARM_ACKNOWLEDGE message handler
//

void CAlarmView::OnAlarmAcknowledge() 
{
   int iItem;
   DWORD i, dwNumAlarms, *pdwAlarmList, dwResult;

   dwNumAlarms = m_wndListCtrl.GetSelectedCount();
   pdwAlarmList = (DWORD *)malloc(sizeof(DWORD) * dwNumAlarms);

   iItem = m_wndListCtrl.GetNextItem(-1, LVNI_SELECTED);
   for(i = 0; (iItem != -1) && (i < dwNumAlarms); i++)
   {
      pdwAlarmList[i] = m_wndListCtrl.GetItemData(iItem);
      iItem = m_wndListCtrl.GetNextItem(iItem, LVNI_SELECTED);
   }

   dwResult = DoRequestArg2(AcknowledgeAlarms, (void *)dwNumAlarms, pdwAlarmList,
                            _T("Acknowledging alarms..."));
   if (dwResult != RCC_SUCCESS)
      theApp.ErrorBox(dwResult, _T("Cannot acknowledge alarm: %s"));
   free(pdwAlarmList);
}


//
// Alarm termination worker function
//

static DWORD TerminateAlarms(DWORD dwNumAlarms, DWORD *pdwAlarmList)
{
   DWORD i, dwResult = RCC_SUCCESS;

   for(i = 0; (i < dwNumAlarms) && (dwResult == RCC_SUCCESS); i++)
      dwResult = NXCTerminateAlarm(g_hSession, pdwAlarmList[i]);
   return dwResult;
}


//
// WM_COMMAND::ID_ALARM_TERMINATE message handler
//

void CAlarmView::OnAlarmTerminate() 
{
   int iItem;
   DWORD i, dwNumAlarms, *pdwAlarmList, dwResult;

   dwNumAlarms = m_wndListCtrl.GetSelectedCount();
   pdwAlarmList = (DWORD *)malloc(sizeof(DWORD) * dwNumAlarms);

   iItem = m_wndListCtrl.GetNextItem(-1, LVNI_SELECTED);
   for(i = 0; (iItem != -1) && (i < dwNumAlarms); i++)
   {
      pdwAlarmList[i] = m_wndListCtrl.GetItemData(iItem);
      iItem = m_wndListCtrl.GetNextItem(iItem, LVNI_SELECTED);
   }

   dwResult = DoRequestArg2(TerminateAlarms, (void *)dwNumAlarms, pdwAlarmList,
                            _T("Terminating alarms..."));
   if (dwResult != RCC_SUCCESS)
      theApp.ErrorBox(dwResult, _T("Cannot terminate alarm: %s"));
   free(pdwAlarmList);
}


//
// Alarm deletion worker function
//

static DWORD DeleteAlarms(DWORD dwNumAlarms, DWORD *pdwAlarmList)
{
   DWORD i, dwResult = RCC_SUCCESS;

   for(i = 0; (i < dwNumAlarms) && (dwResult == RCC_SUCCESS); i++)
      dwResult = NXCDeleteAlarm(g_hSession, pdwAlarmList[i]);
   return dwResult;
}


//
// WM_COMMAND::ID_ALARM_DELETE message handler
//

void CAlarmView::OnAlarmDelete() 
{
   int iItem;
   DWORD i, dwNumAlarms, *pdwAlarmList, dwResult;

   dwNumAlarms = m_wndListCtrl.GetSelectedCount();
   pdwAlarmList = (DWORD *)malloc(sizeof(DWORD) * dwNumAlarms);

   iItem = m_wndListCtrl.GetNextItem(-1, LVNI_SELECTED);
   for(i = 0; (iItem != -1) && (i < dwNumAlarms); i++)
   {
      pdwAlarmList[i] = m_wndListCtrl.GetItemData(iItem);
      iItem = m_wndListCtrl.GetNextItem(iItem, LVNI_SELECTED);
   }

   dwResult = DoRequestArg2(DeleteAlarms, (void *)dwNumAlarms, pdwAlarmList,
                            _T("Deleting alarms..."));
   if (dwResult != RCC_SUCCESS)
      theApp.ErrorBox(dwResult, _T("Cannot acknowledge alarm: %s"));
   free(pdwAlarmList);
}


//
// Calculate width of text
//

int CAlarmView::GetTextWidth(WCHAR *pszText)
{
	CDC *pdc;
	CFont *pOldFont;
	int nRet;

	pdc = GetDC();
   pOldFont = pdc->SelectObject(&m_fontSmall);
	nRet = pdc->GetTextExtent(pszText, wcslen(pszText)).cx;
	pdc->SelectObject(pOldFont);
	ReleaseDC(pdc);
	return nRet;
}
