#include <windows.h>
#include <tchar.h>

// Thread that starts a user interface in a desktop
// Why a thread is required?
// "The SetThreadDesktop function will fail if the calling thread has any windows or hooks on its current desktop" - msnd
DWORD WINAPI DesktopUIStart(LPVOID hNewDesktop)
{
  // Set desktop of this thread
  SetThreadDesktop((HDESK)hNewDesktop);

  // Create a window, dialog, etc
  MessageBox(NULL,_T("Hello desktop!\r\n Click OK to switch back to default desktop"),_T("Test"),MB_ICONINFORMATION);

  return 0;
}


bool Switch()
{
  // Get a handle of the current desktop for switching back later
  HDESK hDeskCurrent = GetThreadDesktop(GetCurrentThreadId());

  // Create a new desktop
  HDESK hDeskNew = CreateDesktop(_T("[Example-SwitchDesktop]"),NULL,NULL,0,GENERIC_ALL,NULL);
  if(hDeskNew == NULL)
  {
      MessageBox(0,_T("Could not create a desktop"),_T("Error"), MB_ICONERROR);
      return false;
  }


  // Create a thread that start any user interface required
  DWORD tid;
  HANDLE hThread = CreateThread(0,0,DesktopUIStart,hDeskNew,0,&tid);
  // Switch to a new desktop
  SwitchDesktop(hDeskNew);
  // Wait until thread finishes
  WaitForSingleObject(hThread,INFINITE);
  // And return to default desktop
  SwitchDesktop(hDeskCurrent);

  return true;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    Switch();
    return 0;
}
