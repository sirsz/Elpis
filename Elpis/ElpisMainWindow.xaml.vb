Imports System.Data
Imports System.Windows.MessageBox
Imports System.Windows.MessageBoxResult
Imports System.IO

Public Class ElpisMainWindow


    Dim context_ As Context

    ' Constructor
    Sub New(ByRef context As Context)

        ' This call is required by the designer.
        InitializeComponent()
        ' Add any initialization after the InitializeComponent() call.
        context_ = context

    End Sub


    Sub UpdateContext()
        Log("Info: Aktualizacja kontekstu")
        textBoxInfoHospital.Text = My.Application.config_.SettingsHospitalName_
        textBoxInfoWard.Text = context_.currWard_.Get_f_Name

        If context_.currWard_.Get_f_Id > -1 Then
            textBoxInfoWard.Text = context_.currWard_.Get_f_Name
        Else
            textBoxInfoWard.Text = String.Empty
        End If

        If context_.currVisit_.Get_f_Id > -1 Then
            textBoxInfoVisit.Text = context_.currVisit_.Get_f_Id
        Else
            textBoxInfoVisit.Text = String.Empty
        End If

        If context_.currLabTest_.Get_f_Id > -1 Then
            textBoxInfoLabTest.Text = context_.currLabTest_.Get_f_Id
        Else
            textBoxInfoLabTest.Text = String.Empty
        End If


        textBoxInfoHospital.Text = My.Application.config_.SettingsHospitalName_
        ' buttonNavNew is disabled when ward tab is selected.

        Select Case context_.TabSelected_
            Case Context.TabSelected.Hospital
                textBoxInfoContext.Text = "Szpital"
            Case Context.TabSelected.Ward
                textBoxInfoContext.Text = "Oddział"
            Case Context.TabSelected.Room
                textBoxInfoContext.Text = "Sala szpitalna"
            Case Context.TabSelected.Staff
                textBoxInfoContext.Text = "Personel"
            Case Context.TabSelected.Patient
                textBoxInfoContext.Text = "Pacjent"
            Case Context.TabSelected.Visit
                textBoxInfoContext.Text = "Wizyta"
            Case Context.TabSelected.LabTest
                textBoxInfoContext.Text = "Test"
            Case Context.TabSelected.Duty
                textBoxInfoContext.Text = "Dyżur"
            Case Context.TabSelected.None
                textBoxInfoContext.Text = String.Empty
            Case Else
                textBoxInfoContext.Text = String.Empty
        End Select
        Log("Info: Kontekst programu został zaktualizowany")
    End Sub

    ' Logging interface - Uses Separate logger class
    Public Sub Log(ByVal newLog As String)
        Toolbox.Log(newLog)
        UpdateLogLabel(newLog)
    End Sub


    Private Sub UpdateLogLabel(ByVal newLog As String)
        labelLog.Content = newLog
    End Sub

    Private Sub buttonMenCalculate_Click(sender As Object, e As RoutedEventArgs) Handles buttonMenCalculate.Click

        Log("Info: Obliczanie ścieżek zakażeń...")
        'executeCmdCommand() ' For the testing purposs
        ExportAllData()
        executeRAnalysisScript()

        ' Wyswlietlenie okna z wynikami:
        Dim results_visualization_window_report As New WindowReport3

        results_visualization_window_report.Show()
        results_visualization_window_report.Activate()

        Log("Info: Obliczanie ścieżek zakażeń - zakończono")

    End Sub

    Sub executeCmdCommand()
        ' Way to recognize if Tab is selected - in front of UI.
        'If TabItem4Personel.IsSelected() Then
        '    Log(buttonMenCalculate.Content.ToString + "TabItem4Personel is selected")
        'Else
        '    Log(buttonMenCalculate.Content.ToString + "TabItem4Personel is not focuesed")
        'End If 
        ' Option 1. Works but window is not escaping
        'Process.Start("cmd", "/c echo This is R analysis report > Elpis-DataAnalysisEngine_Report.txt")
        'Process.Start("cmd", "/c timeout 5")
        Dim report_generation_time = DateTime.Now.ToString("[yyyy/MM/dd HH:mm:ss]")
        Dim cmd_r_command = "/c echo Trwa analiza danych - wyznaczanie ścieżek zakażeń (R). Proszę czekać.. & timeout 7 & echo " &
        " [R analysis report " & report_generation_time & "] > SIRS-Z_R_Report.txt"

        Process.Start("cmd", cmd_r_command)

        ' Option 2. Not works
        'Shell("C:\command.bat")
        'Dim params As String = "echo This is R analysis report > Elpis-DataAnalysisEngine_Report2.txt"
        'Dim myProcess As New ProcessStartInfo
        'myProcess.FileName = "cmd.exe"
        'myProcess.Arguments = params
        'myProcess.WindowStyle = ProcessWindowStyle.Hidden
        'Process.Start(myProcess)

        ' Option 3  Is not working 

        'Dim params As String = "echo This is R analysis report > Elpis-DataAnalysisEngine_Report2.txt"
        'Dim myProcess As New ProcessStartInfo
        'myProcess.FileName = "cmd.exe"
        'myProcess.Arguments = params
        'myProcess.WindowStyle = ProcessWindowStyle.Hidden
        'Process.Start(myProcess)

        'Option 4.
        'StartProcess()

        ' Option 5. - WORKS 
        'Run_Process("cmd", "/c echo This is R analysis report > Elpis-DataAnalysisEngine_Report4.txt ")
        'Run_Process("cmd", "/c timeout 5 ")

    End Sub

    Sub executeRAnalysisScript()
        Dim report_generation_time = DateTime.Now.ToString("[yyyy/MM/dd HH:mm:ss]")
        'Dim cmd_r_command = "/c echo [SIRS-Z " & report_generation_time & "] Trwa analiza danych - wyznaczanie ścieżek zakażeń (R). Proszę czekać.. " &
        '" & start siralgorytm\RCommand.bat > siralgorytm\RCommand.log"
        Dim home_directory = Directory.GetCurrentDirectory()
        Dim script_name = "RCommand.bat"
        Dim working_directory = "siralgorytm"
        'Dim script_full_path = current_directory + "\" + working_directory + "\" + script_name
        ' Dim full_command = script_name & "> " & script_name & ".log" ' Is not working!

        ChDir(working_directory)
        Process.Start(script_name)
        ChDir(home_directory)

    End Sub


    Sub StartProcess()
        Dim procCMDWin As New Process
        Dim params As String = "/c echo This Is R analysis report > t"

        procCMDWin.StartInfo.RedirectStandardOutput = True
        procCMDWin.StartInfo.RedirectStandardInput = True
        procCMDWin.StartInfo.CreateNoWindow = True

        procCMDWin.StartInfo.UseShellExecute = False


        procCMDWin.StartInfo.FileName = "cmd.exe"
        procCMDWin.StartInfo.WorkingDirectory = Directory.GetCurrentDirectory()
        procCMDWin.StartInfo.Arguments = params

        procCMDWin.Start()

        procCMDWin.BeginOutputReadLine()
        'Do Until (procCMDWin.HasExited)
        'Loop

        procCMDWin.Dispose()
    End Sub


    ' [ Run Process Function ]
    '
    ' // By Elektro H@cker
    '
    ' Examples :
    '
    ' MsgBox(Run_Process("Process.exe")) 
    ' MsgBox(Run_Process("Process.exe", "Arguments"))
    ' MsgBox(Run_Process("CMD.exe", "/C Dir /B", True))
    ' MsgBox(Run_Process("CMD.exe", "/C @Echo OFF & For /L %X in (0,1,50000) Do (Echo %X)", False, False))
    ' MsgBox(Run_Process("CMD.exe", "/C Dir /B /S %SYSTEMDRIVE%\*", , False, 500))
    ' If Run_Process("CMD.exe", "/C Dir /B", True).Contains("File.txt") Then MsgBox("File found")

    Private Function Run_Process(ByVal Process_Name As String,
                             Optional Process_Arguments As String = Nothing,
                             Optional Read_Output As Boolean = False,
                             Optional Process_Hide As Boolean = False,
                             Optional Process_TimeOut As Integer = 3600 * 1000) ' 3600 * 1000  = 1h = 60 min * 60 sec * 1000 milisec 

        ' Returns True if "Read_Output" argument is False and Process was finished OK
        ' Returns False if ExitCode is not "0"
        ' Returns Nothing if process can't be found or can't be started
        ' Returns "ErrorOutput" or "StandardOutput" (In that priority) if Read_Output argument is set to True.

        Try

            Dim My_Process As New Process()
            Dim My_Process_Info As New ProcessStartInfo()

            My_Process_Info.FileName = Process_Name ' Process filename
            My_Process_Info.Arguments = Process_Arguments ' Process arguments
            My_Process_Info.CreateNoWindow = Process_Hide ' Show or hide the process Window
            My_Process_Info.UseShellExecute = False ' Don't use system shell to execute the process
            My_Process_Info.RedirectStandardOutput = Read_Output '  Redirect (1) Output
            My_Process_Info.RedirectStandardError = Read_Output ' Redirect non (1) Output
            My_Process_Info.WindowStyle = ProcessWindowStyle.Hidden
            My_Process.EnableRaisingEvents = True ' Raise events
            My_Process.StartInfo = My_Process_Info
            My_Process.Start() ' Run the process NOW

            My_Process.WaitForExit(Process_TimeOut) ' Wait X ms to kill the process (Default value is 999999999 ms which is 277 Hours)

            Dim ERRORLEVEL = My_Process.ExitCode ' Stores the ExitCode of the process
            If Not ERRORLEVEL = 0 Then Return False ' Returns the Exitcode if is not 0

            If Read_Output = True Then
                Dim Process_ErrorOutput As String = My_Process.StandardOutput.ReadToEnd() ' Stores the Error Output (If any)
                Dim Process_StandardOutput As String = My_Process.StandardOutput.ReadToEnd() ' Stores the Standard Output (If any)
                ' Return output by priority
                If Process_ErrorOutput IsNot Nothing Then Return Process_ErrorOutput ' Returns the ErrorOutput (if any)
                If Process_StandardOutput IsNot Nothing Then Return Process_StandardOutput ' Returns the StandardOutput (if any)
            End If

        Catch ex As Exception
            'MsgBox(ex.Message)
            Return Nothing ' Returns nothing if the process can't be found or started.
        End Try

        Return True ' Returns True if Read_Output argument is set to False and the process finished without errors.

    End Function

    Private Sub MIF_Close_Program_Click(sender As Object, e As RoutedEventArgs) Handles MIF_Close_Program.Click
        System.Environment.Exit(0)
    End Sub

    Private Sub buttonNavSearch_Click(sender As Object, e As RoutedEventArgs) Handles buttonNavSearch.Click
        Log(buttonNavSearch.Content.ToString)

        Log(buttonMenCalculate.Content.ToString)
        UpdateContext()

        If TabItem2Ward.IsSelected() Then
            ITabItem2Ward_Search()
        ElseIf TabItem3Room.IsSelected Then
            ITabItem3Room_Search()
        ElseIf TabItem4Personel.IsSelected Then
            ITabItem4Personel_Search()
            'ElseIf TabItem5Patient.IsSelected Then
            '    ITabItem5Patient_Search()
        ElseIf TabItem6Visit.IsSelected Then
            ITabItem6Visit_Search()
        ElseIf TabItem7LabTest.IsSelected Then
            ITabItem7LabTest_Search()
        ElseIf TabItem8Duty.IsSelected Then
            ITabItem8Duty_Search()
        End If
    End Sub

    Private Sub buttonNavCleanForm_Click(sender As Object, e As RoutedEventArgs) Handles buttonNavCleanForm.Click
        Log(buttonNavCleanForm.Content.ToString)
        context_.TabSelected_ = Context.TabSelected.None
        context_.contextID_ = -1

        If TabItem2Ward.IsSelected() Then
            ITabItem2Ward_Clean()
        ElseIf TabItem3Room.IsSelected Then
            ITabItem3Room_Clean()
        ElseIf TabItem4Personel.IsSelected Then
            ITabItem4Personel_Clean()
            'ElseIf TabItem5Patient.IsSelected Then
            '    ITabItem5Patient_Clean()
        ElseIf TabItem6Visit.IsSelected Then
            ITabItem6Visit_Clean()
        ElseIf TabItem7LabTest.IsSelected Then
            ITabItem7LabTest_Clean()
        ElseIf TabItem8Duty.IsSelected Then
            ITabItem8Duty_Clean()
        End If
        UpdateContext()

    End Sub

    Private Sub buttonNavNew_Click(sender As Object, e As RoutedEventArgs) Handles buttonNavNew.Click
        Log(buttonNavNew.Content.ToString)
        If Not context_.TabSelected_ = Context.TabSelected.None Then
            Log("Info: Kontekst programu nie został zdefiniowany - operacja nie zostanie wykonana.")
        End If

        If TabItem2Ward.IsSelected() Then
            ITabItem2Ward_New()
        ElseIf TabItem3Room.IsSelected Then
            ITabItem3Room_New()
        ElseIf TabItem4Personel.IsSelected Then
            ITabItem4Personel_New()
            'ElseIf TabItem5Patient.IsSelected Then
            '    ITabItem5Patient_New()
        ElseIf TabItem6Visit.IsSelected Then
            ITabItem6Visit_New()
        ElseIf TabItem7LabTest.IsSelected Then
            ITabItem7LabTest_New()
        ElseIf TabItem8Duty.IsSelected Then
            ITabItem8Duty_New()
        End If
    End Sub

    Private Sub buttonNavSave_Click(sender As Object, e As RoutedEventArgs) Handles buttonNavSave.Click
        ' 2017-01-22 Edit: Save same will use Update
        Log(buttonNavSave.Content.ToString)
        If TabItem2Ward.IsSelected() Then
            ITabItem2Ward_Update()
        ElseIf TabItem3Room.IsSelected Then
            ITabItem3Room_Update()
        ElseIf TabItem4Personel.IsSelected Then
            ITabItem4Personel_Update()
            'ElseIf TabItem5Patient.IsSelected Then
            '    ITabItem5Patient_New()
        ElseIf TabItem6Visit.IsSelected Then
            ITabItem6Visit_Update()
        ElseIf TabItem7LabTest.IsSelected Then
            ITabItem7LabTest_Update()
        ElseIf TabItem8Duty.IsSelected Then
            ITabItem8Duty_Update()
        Else
            Log("Info: Nieoczekiwana opcja - operacja nie zostanie wykonana.")
        End If

    End Sub

    Private Sub buttonNavDelete_Click(sender As Object, e As RoutedEventArgs) Handles buttonNavDelete.Click
        Log(buttonNavDelete.Content.ToString)
        Log(buttonNavCleanForm.Content.ToString)
        If context_.TabSelected_ = Context.TabSelected.None Then
            Log("Info: Operacja usunięcia anulowana - kontekst programu jest pusty. Znajdź wpis do usunięcia.")
            Return
        End If

        If TabItem2Ward.IsSelected() Then
            ITabItem2Ward_Delete()
        ElseIf TabItem3Room.IsSelected Then
            ITabItem3Room_Delete()
        ElseIf TabItem4Personel.IsSelected Then
            ITabItem4Personel_Delete()
            'ElseIf TabItem5Patient.IsSelected Then
            '    ITabItem5Patient_Delete()
        ElseIf TabItem6Visit.IsSelected Then
            ITabItem6Visit_Delete()
        ElseIf TabItem7LabTest.IsSelected Then
            ITabItem7LabTest_Delete()
        ElseIf TabItem8Duty.IsSelected Then
            ITabItem8Duty_Delete()
        End If
    End Sub

    Private Sub buttonNavPrevious_Click(sender As Object, e As RoutedEventArgs) Handles buttonNavPrevious.Click
        Log(buttonNavPrevious.Content.ToString)
        If Not MainWindow_listView.SelectedItem Is Nothing Then
            If MainWindow_listView.SelectedIndex > 0 Then
                MainWindow_listView.SelectedIndex = MainWindow_listView.SelectedIndex - 1
            Else
                Log("Początek listy")
            End If
        End If
    End Sub

    Private Sub buttonNavNext_Click(sender As Object, e As RoutedEventArgs) Handles buttonNavNext.Click
        Log(buttonNavNext.Content.ToString)
        If Not MainWindow_listView.SelectedItem Is Nothing Then
            If MainWindow_listView.SelectedIndex < MainWindow_listView.Items.Count - 1 Then
                MainWindow_listView.SelectedIndex = MainWindow_listView.SelectedIndex + 1
            Else
                Log("Koniec listy")
            End If
        End If
    End Sub


    Private Sub MIF_Open_Click(sender As Object, e As RoutedEventArgs) Handles MIF_Open.Click
        Toolbox.Log("Opcja Otwórz - nieaktywna w tej wersji")
    End Sub

    Private Sub MIF_New_Click(sender As Object, e As RoutedEventArgs) Handles MIF_New.Click
        Toolbox.Log("Opcja Nowy - nieaktywna w tej wersji")
    End Sub

    Private Sub MIE_ClearAll_Click(sender As Object, e As RoutedEventArgs) Handles MIE_ClearAll.Click
        Toolbox.Log("Opcja Wyczyść - nieaktywna w tej wersji")
    End Sub

    Private Sub MIV_Settings_Click(sender As Object, e As RoutedEventArgs) Handles MIV_Settings.Click
        Log(MIV_Settings.Header.ToString)
        Dim window As New WindowSettings

        window.Show()
        window.Activate()
    End Sub

    Private Sub MenuItem_Click(sender As Object, e As RoutedEventArgs)
        Log(MIV_Settings.Header.ToString)
        Dim window As New WindowSettingsImportData
        window.Show()
        window.Activate()
    End Sub


    Private Sub MIH_ViewHelp_Click(sender As Object, e As RoutedEventArgs) Handles MIH_ViewHelp.Click
        Log(MIH_ViewHelp.Header.ToString)
        MsgBox("Pomoc do programu Elips" & Environment.NewLine & Environment.NewLine & "www: http://www.sirsz.pl" & Environment.NewLine & "E-mail: biuro@sirsz.org")
    End Sub

    Private Sub MIP_About_Click(sender As Object, e As RoutedEventArgs) Handles MIP_About.Click
        Log(MIH_ViewHelp.Header.ToString)
        MsgBox("Elips - Manager danych Systemu Informatycznego Redukcji Zakażeń Szpitalnych" & Environment.NewLine & "v 1.1.11. Beta" & Environment.NewLine & "SIRS-Z © 2016 - 2017" & Environment.NewLine & "WWW: http://www.sirsz.pl/")
    End Sub


    Private Sub Fill_ListView(dataTable As DataTable)
        MainWindow_listView.Items.Clear()
        For Each row As DataRow In dataTable.Rows
            Dim listRow As String = ""
            For Each item In row.ItemArray
                listRow += item.ToString() + "|"
            Next
            MainWindow_listView.Items.Add(listRow)
        Next
    End Sub

    Private Sub ITabItem2Ward_Search()
        Log("Info: Wyszukiwanie oddziału..")
        ' Update program context
        context_.TabSelected_ = Context.TabSelected.Ward
        UpdateContext()
        ' Get data from DB
        ITabItem2Ward_UpdateContextWardFromForm()
        Dim querry As String = context_.currWard_.FormQuerry_Select()
        Dim result = My.Application.DatabaseSqliteInterface_.ExecuteQuerry(querry)
        Fill_ListView(result)
        result.Clear()
        result.Dispose()
        Log("Info: Zakończono wyszukiwanie oddziałów. Liczba znalezionych pozycji: " & result.Rows.Count)
    End Sub

    Sub ITabItem3Room_Search()
        Log("Info: Wyszukiwanie sal..")
        ' Update program context
        context_.TabSelected_ = Context.TabSelected.Room
        UpdateContext()
        ' Get data from DB

        ITabItem3Room_UpdateContextRoomFromForm()
        Dim querry As String = context_.currRoom_.FormQuerry_Select()
        Dim result = My.Application.DatabaseSqliteInterface_.ExecuteQuerry(querry)
        Fill_ListView(result)
        result.Clear()
        result.Dispose()
        Log("Info: Zakończono wyszukiwanie sal. Liczba znalezionych pozycji: " & result.Rows.Count)
    End Sub

    Sub ITabItem4Personel_Search()
        Log("Info: Wyszukiwanie personelu..")
        ' Update program context
        context_.TabSelected_ = Context.TabSelected.Staff
        UpdateContext()
        ' Get data from DB

        ITabItem4Personel_UpdateContextHospitalStuffFromForm()

        Dim querry As String = context_.currHospitalStaff_.FormQuerry_Select()
        Dim result = My.Application.DatabaseSqliteInterface_.ExecuteQuerry(querry)
        Fill_ListView(result)
        result.Clear()
        result.Dispose()
        Log("Info: Zakończono wyszukiwanie pracowników. Liczba znalezionych pozycji: " & result.Rows.Count)
    End Sub

    Sub ITabItem6Visit_Search()
        Log("Info: Wyszukiwanie wizyty..")
        context_.TabSelected_ = Context.TabSelected.Visit
        UpdateContext()
        ITabItem6Visit_UpdateContextVisitFromForm()
        Dim querry As String = context_.currVisit_.FormQuerry_Select()
        Dim result = My.Application.DatabaseSqliteInterface_.ExecuteQuerry(querry)
        Fill_ListView(result)
        result.Clear()
        result.Dispose()
        Log("Info: Zakończono wyszukiwanie wizyt. Liczba znalezionych pozycji: " & result.Rows.Count)
    End Sub

    Sub ITabItem7LabTest_Search()
        Log("Info: Wyszukiwanie badania..")
        context_.TabSelected_ = Context.TabSelected.LabTest
        UpdateContext()
        ITabItem7LabTest_UpdateContextLabTestFromForm()
        Dim querry As String = context_.currLabTest_.FormQuerry_Select()
        Dim result = My.Application.DatabaseSqliteInterface_.ExecuteQuerry(querry)
        Fill_ListView(result)
        result.Clear()
        result.Dispose()
        Log("Info: Zakończono wyszukiwanie badań. Liczba znalezionych pozycji: " & result.Rows.Count)
    End Sub

    Sub ITabItem8Duty_Search()
        Log("Info: Wyszukiwanie dyżuru..")
        context_.TabSelected_ = Context.TabSelected.Duty
        UpdateContext()
        ITabItem8Duty_UpdateContextDutyFromForm()
        Dim querry As String = context_.currDuty_.FormQuerry_Select()
        Dim result = My.Application.DatabaseSqliteInterface_.ExecuteQuerry(querry)
        Fill_ListView(result)
        result.Clear()
        result.Dispose()
        Log("Info: Zakończono wyszukiwanie dyżurów. Liczba znalezionych pozycji: " & result.Rows.Count)
    End Sub

    Private Sub listViewTest_SelectionChanged(sender As Object, e As SelectionChangedEventArgs) Handles MainWindow_listView.SelectionChanged
        Log("[Selektor] Wybór zakładki zmienił się.")
        If Not MainWindow_listView.SelectedItem Is Nothing Then
            Log(MainWindow_listView.SelectedIndex().ToString)
            Log(MainWindow_listView.SelectedItem().ToString)
            Dim selected_item_as_string = MainWindow_listView.SelectedItem().ToString()
            Dim parts As String() = SplitString(selected_item_as_string)

            If TabItem2Ward.IsSelected() Then
                ITabItem2Ward_FillForm(selected_item_as_string)
            ElseIf TabItem3Room.IsSelected Then
                ITabItem3Room_FillForm(selected_item_as_string)
            ElseIf TabItem4Personel.IsSelected Then
                ITabItem4Personel_FillForm(selected_item_as_string)
                'ElseIf TabItem5Patient.IsSelected Then
                '   ITabItem5Patient_FillForm(parts)
            ElseIf TabItem6Visit.IsSelected Then
                ITabItem6Visit_FillForm(selected_item_as_string)
            ElseIf TabItem7LabTest.IsSelected Then
                ITabItem7LabTest_FillForm(selected_item_as_string)
            ElseIf TabItem8Duty.IsSelected Then
                ITabItem8Duty_FillForm(selected_item_as_string)
            Else
                Log("Zakładka nieznana - akcja zignorowana.")
            End If

            UpdateContext()
        End If
    End Sub

    Private Function SplitString(ByVal s As String) As String()
        Dim parts As String() = s.Split(New Char() {"|"c})

        ' Loop through result strings with For Each.
        Dim part As String
        For Each part In parts
            Console.WriteLine(part)
        Next
        Return parts
    End Function

    ' CLEAN BUTTON SEVICE METHODS - BEGIN
    Private Sub ITabItem2Ward_Clean()
        Log("Czyszczenie formularza Oddział")

        Ft2TextBoxWardID.Text = String.Empty
        Ft2TextBoxWardName.Text = String.Empty
        Ft2TextBoxWardMaxNumberOfBeds.Text = String.Empty
        Ft2TextBoxMeanOccupancy.Text = String.Empty
        Ft2WardType_ComboBox.Text = String.Empty
        context_.currWard_.Clear()

    End Sub

    Sub ITabItem3Room_Clean()
        Log("Czyszczenie formularza Sal")
        Ft3TextBoxRoomNumber.Text = String.Empty
        Ft3TextBoxRoomId.Text = String.Empty
        Ft3TextBoxMeanOccupancy.Text = String.Empty
        Ft3TextBoxMaxNumberOfBeds.Text = String.Empty
        ' Room.Type - deleted 2017.07.20
        'Ft3RoomType_ComboBox.Text = String.Empty
        context_.currRoom_.Clear()

    End Sub

    Sub ITabItem4Personel_Clean()
        Log("Czyszczenie formularza Personelu")
        Ft4PersonelHospID_TextBox.Text = String.Empty
        Ft4PersonelName_TextBox.Text = String.Empty
        Ft4PersonelFunction_ComboBox.Text = String.Empty
        context_.currHospitalStaff_.Clear()

    End Sub

    Sub ITabItem5Patient_Clean()
        Log("Czyszczenie formularza Pacjenta")
        Ft5PatientHospID_TextBox.Text = String.Empty
        Ft5PatientHospIdent_Textbox.Text = String.Empty
        Ft5PatientHospAdditionalInfo_Textbox.Text = String.Empty

        Ft5PatientHospRegisterType_ComboBox.Text = String.Empty
    End Sub

    Sub ITabItem6Visit_Clean()
        Log("Czyszczenie formularza Wizyt")
        'Ft6Visit_HospID_TextBox.Text = String.Empty
        Ft6Visit_PatientHospID_TextBox.Text = String.Empty
        Ft6Visit_HospPlaceID_TextBox.Text = String.Empty
        Ft6Visit_SigninDate_TextBox.Text = String.Empty
        Ft6Visit_SignoutDate_TextBox.Text = String.Empty
        Ft6Visit_Name_TextBox.Text = String.Empty
        context_.currVisit_.Clear()

    End Sub

    Sub ITabItem7LabTest_Clean()
        Log("Czyszczenie formularza Test")

        Ft7LabTest_LabTestID_TextBox.Text = String.Empty
        Ft7LabTest_PatientHospID_TextBox.Text = String.Empty

        Ft7LabTest_P1OrderDate_TextBox.Text = String.Empty
        Ft7LabTest_P1Result_TextBox.Text = String.Empty
        Ft7LabTest_P1Level_TextBox.Text = String.Empty
        Ft7LabTest_P1Root_TextBox.Text = String.Empty

        Ft7LabTest_P2OrderDate_TextBox.Text = String.Empty
        Ft7LabTest_P2Result_TextBox.Text = String.Empty
        Ft7LabTest_P2Root_TextBox.Text = String.Empty
        Ft7LabTest_P2Level_TextBox.Text = String.Empty
        context_.currLabTest_.Clear()

    End Sub

    Sub ITabItem8Duty_Clean()
        Log("Czyszczenie formularza Dyżur")

        Ft8Duty_Date_DatePicker.Text = String.Empty
        Ft8Duty_PersonelHospID_TextBox.Text = String.Empty

    End Sub
    ' CLEAR BUTTON SEVICE METHODS - END

    ' FILL FORM METHODS - BEGIN
    Sub ITabItem2Ward_FillForm(ByVal ward_as_a_string_line As String)
        Log("ITabItem2Ward_FillForm")
        ' Check the program context and if correct update data ID
        If Not context_.TabSelected_ = Context.TabSelected.Ward Then
            Log("ITabItem2Ward_FillForm: context has been changed")
            Return
        End If
        context_.currWard_.Parse(ward_as_a_string_line)
        context_.contextID_ = context_.currWard_.Get_d_Id()
        ITabItem2Ward_UpdateFormFromContextWard()
    End Sub

    Sub ITabItem3Room_FillForm(ByVal room_as_a_string_linerts As String)
        Log("ITabItem3Room_FillForm")
        ' Check the program context and if correct update data ID
        If Not context_.TabSelected_ = Context.TabSelected.Room Then
            Log("ITabItem3Room_FillForm: Context has been changed")
            Return
        End If
        context_.currRoom_.Parse(room_as_a_string_linerts)
        context_.contextID_ = context_.currRoom_.Get_d_Id()
        ITabItem3Room_UpdateFormFromContextRoom()
    End Sub

    Sub ITabItem4Personel_FillForm(ByVal hospital_staff_as_a_string_linerts As String)
        Log("Aktualizacja formularza: Personel")
        ' Check the program context and if correct update data ID
        If Not context_.TabSelected_ = Context.TabSelected.Staff Then
            Log("IInfo: Kontekst: Personel - zmienił się")
            Return
        End If
        context_.currHospitalStaff_.Parse(hospital_staff_as_a_string_linerts)
        context_.contextID_ = context_.currHospitalStaff_.Get_d_Id()
        ITabItem4Personel_UpdateFormFromContextHospitalStuff()
    End Sub

    Sub ITabItem6Visit_FillForm(ByVal visit_as_a_string_linerts As String)
        Log("Aktualizacja formularza: Wizyta")
        ' Check the program context and if correct update data ID
        If Not context_.TabSelected_ = Context.TabSelected.Visit Then
            Log("Info: Kontekst: Wizyta - zmienił się")
            Return
        End If
        context_.currVisit_.Parse(visit_as_a_string_linerts)
        context_.contextID_ = context_.currVisit_.Get_d_Id()
        ITabItem6Visit_UpdateFormFromContextVisit()
    End Sub

    Sub ITabItem7LabTest_FillForm(ByVal visit_as_a_string_linerts As String)
        Log("Aktualizacja formularza: Test")
        ' Check the program context and if correct update data ID
        If Not context_.TabSelected_ = Context.TabSelected.LabTest Then
            Log("Info: Kontekst: Test zmienił się")
            Return
        End If
        context_.currLabTest_.Parse(visit_as_a_string_linerts)
        context_.contextID_ = context_.currLabTest_.Get_d_Id()
        ITabItem7LabTest_UpdateFormFromContextLabTest()
    End Sub

    ' SINCE 2017-07-05
    Sub ITabItem8Duty_FillForm(ByVal duty_as_a_string_linerts As String)
        Log("Aktualizacja formularza: Dyżur")
        ' Check the program context and if correct update data ID
        If Not context_.TabSelected_ = Context.TabSelected.Duty Then
            Log("Info: Kontekst: Dyżur zmienił się")
            Return
        End If
        context_.currDuty_.Parse(duty_as_a_string_linerts)
        context_.contextID_ = context_.currDuty_.Get_d_Id()
        ITabItem8Duty_UpdateFormFromContextDuty()
    End Sub
    ' FILL FORM METHODS - END

    ' UPDATE FORM METHODS - BEGIN


    ' WARD FORM <-> CONTEXT
    Sub ITabItem2Ward_UpdateContextWardFromForm()

        'Log("ITabItem1Ward_UpdateContextWardFromForm")
        Dim WardID As String = Ft2TextBoxWardID.Text
        Dim WardName As String = Ft2TextBoxWardName.Text
        Dim MaxNumberOfBeds As String = Ft2TextBoxWardMaxNumberOfBeds.Text
        Dim MeanOccupancy As String = Ft2TextBoxMeanOccupancy.Text
        Dim WardType As String = "Uninitialized"
        If Ft2WardType_ComboBox.SelectedIndex > -1 Then
            'WardType = Ft2WardType_ComboBox.SelectedItem.ToString
            WardType = My.Application.config_.WardTypes_hashtable.Item(Ft2WardType_ComboBox.SelectedIndex)
        End If

        context_.currWard_.ValidateAndUpdate(WardID, WardName, WardType, MaxNumberOfBeds, MeanOccupancy)
        context_.currWard_.d_id_ = context_.contextID_
    End Sub
    Sub ITabItem2Ward_UpdateFormFromContextWard()
        'Log("ITabItem1Ward_UpdateFormFromContextWard")
        Ft2TextBoxWardID.Text = context_.currWard_.Get_f_Id
        Ft2TextBoxWardName.Text = context_.currWard_.Get_f_Name
        Ft2TextBoxWardMaxNumberOfBeds.Text = context_.currWard_.Get_f_bed_number_max_
        Ft2TextBoxMeanOccupancy.Text = context_.currWard_.Get_f_bed_number_mean_
        'Ft2WardType_ComboBox.Text = context_.currWard_.Get_f_Type
        Dim combobox_index_from_hashTable = My.Application.config_.GetHashTable_Id_for_Value(context_.currWard_.Get_f_Type)

        Ft2WardType_ComboBox.SelectedIndex = combobox_index_from_hashTable
        context_.contextID_ = context_.currWard_.d_id_
    End Sub

    ' ROOM FORM <-> CONTEXT
    Sub ITabItem3Room_UpdateContextRoomFromForm()
        'Log("ITabItem1Ward_UpdateContextWardFromForm")
        Dim RoomID As String = Ft3TextBoxRoomId.Text
        Dim RoomNumner As String = Ft3TextBoxRoomNumber.Text
        Dim MaxNumberOfBeds As String = Ft3TextBoxMaxNumberOfBeds.Text
        Dim MeanOccupancy As String = Ft3TextBoxMeanOccupancy.Text

        Dim RoomType As String = "Uninitialized"
        ' Room.Type - deleted 2017.07.20
        'If Ft3RoomType_ComboBox.SelectedIndex > -1 Then
        '    RoomType = Ft3RoomType_ComboBox.SelectedValue.ToString
        'End If
        context_.currRoom_.ValidateAndUpdate(RoomID, RoomNumner, RoomType, MaxNumberOfBeds, MeanOccupancy)
        context_.currWard_.d_id_ = context_.contextID_
    End Sub
    Sub ITabItem3Room_UpdateFormFromContextRoom()
        Ft3TextBoxRoomId.Text = context_.currRoom_.Get_f_Id
        Ft3TextBoxRoomNumber.Text = context_.currRoom_.Get_f_number
        Ft3TextBoxMaxNumberOfBeds.Text = context_.currRoom_.Get_f_bed_number_max
        Ft3TextBoxMeanOccupancy.Text = context_.currRoom_.Get_f_bed_number_mean
        Ft3TextBoxMeanOccupancy.Text = context_.currRoom_.Get_f_bed_number_mean

        ' Room.Type - deleted 2017.07.20
        'Ft3RoomType_ComboBox.Text = context_.currRoom_.Get_f_Type

        context_.contextID_ = context_.currRoom_.d_id_
    End Sub


    ' PERSOEL FORM <-> CONTEXT
    Sub ITabItem4Personel_UpdateContextHospitalStuffFromForm()
        Log("ITabItem4Personel_UpdateContextHospitalStuffFromForm")
        Dim PersonelHospID As String = Ft4PersonelHospID_TextBox.Text
        Dim PersonelName As String = Ft4PersonelName_TextBox.Text
        Dim PersonelFunction As String = "Undefined"
        If Ft4PersonelFunction_ComboBox.SelectedIndex = 0 Then
            PersonelFunction = "Lekarska"
        ElseIf Ft4PersonelFunction_ComboBox.SelectedIndex = 1 Then
            PersonelFunction = "Pielęgniarska"
        End If
        context_.currHospitalStaff_.ValidateAndUpdate(PersonelHospID, PersonelName, PersonelFunction)
        context_.currHospitalStaff_.d_id_ = context_.contextID_
    End Sub
    Sub ITabItem4Personel_UpdateFormFromContextHospitalStuff()
        Ft4PersonelHospID_TextBox.Text = context_.currHospitalStaff_.Get_f_Id
        Ft4PersonelName_TextBox.Text = context_.currHospitalStaff_.Get_f_fullname

        If context_.currHospitalStaff_.Get_f_Type = HospitalStaffType.Lekarska.ToString Then
            Ft4PersonelFunction_ComboBox.SelectedIndex = 0
        ElseIf context_.currHospitalStaff_.Get_f_Type = HospitalStaffType.Pielęgniarska.ToString Then
            Ft4PersonelFunction_ComboBox.SelectedIndex = 1
        Else
            Ft4PersonelFunction_ComboBox.SelectedIndex = -1
        End If

        context_.contextID_ = context_.currHospitalStaff_.d_id_
    End Sub

    ' VISIT FORM <-> CONTEXT
    Sub ITabItem6Visit_UpdateContextVisitFromForm()

        Dim f_patient_id As String = Ft6Visit_PatientHospID_TextBox.Text
        Dim f_id As String = f_patient_id
        'Dim f_patient_id As Integer  ' forgein key of <-> Patient.f_id_
        Dim f_ward_id As String = Ft6Visit_HospPlaceID_TextBox.Text
        Dim f_name As String = Ft6Visit_Name_TextBox.Text
        Dim f_date_in As String = Ft6Visit_SigninDate_TextBox.Text
        Dim f_date_out As String = Ft6Visit_SignoutDate_TextBox.Text

        context_.currVisit_.ValidateAndUpdate(f_id, f_name, f_date_in, f_date_out, f_ward_id, f_patient_id)
        context_.currVisit_.d_id_ = context_.contextID_
    End Sub
    Sub ITabItem6Visit_UpdateFormFromContextVisit()
        Ft6Visit_PatientHospID_TextBox.Text = context_.currVisit_.Get_f_Patient_Id
        Ft6Visit_HospPlaceID_TextBox.Text = context_.currVisit_.f_ward_id_
        Ft6Visit_SigninDate_TextBox.Text = context_.currVisit_.Get_f_Date_In
        Ft6Visit_SignoutDate_TextBox.Text = context_.currVisit_.Get_f_Date_Out
        Ft6Visit_Name_TextBox.Text = context_.currVisit_.Get_f_Name

        context_.contextID_ = context_.currVisit_.d_id_
    End Sub

    ' LAB TEST FORM <-> CONTEXT
    Sub ITabItem7LabTest_UpdateContextLabTestFromForm()

        Dim f_id As String = Ft7LabTest_LabTestID_TextBox.Text
        Dim f_patient_id As String = Ft7LabTest_PatientHospID_TextBox.Text ' forgein key of <-> Patient.f_id_

        Dim f_p1_date As String = Ft7LabTest_P1OrderDate_TextBox.Text
        Dim f_p1_result_bin As String = Ft7LabTest_P1Result_TextBox.Text
        Dim f_p1_result_nominal As String = Ft7LabTest_P1Level_TextBox.Text
        Dim f_p1_kind As String = Ft7LabTest_P1Root_TextBox.Text

        Dim f_p2_date As String = Ft7LabTest_P2OrderDate_TextBox.Text
        Dim f_p2_result_bin As String = Ft7LabTest_P2Result_TextBox.Text
        Dim f_p2_result_nominal As String = Ft7LabTest_P2Level_TextBox.Text
        Dim f_p2_kind As String = Ft7LabTest_P2Root_TextBox.Text

        context_.currLabTest_.ValidateAndUpdate(f_id,
                                                f_p1_date, f_p1_result_bin, f_p1_result_nominal, f_p1_kind,
                                                f_p2_date, f_p2_result_bin, f_p2_result_nominal, f_p2_kind,
                                                f_patient_id)
        context_.currLabTest_.d_id_ = context_.contextID_
    End Sub
    Sub ITabItem7LabTest_UpdateFormFromContextLabTest()

        Ft7LabTest_LabTestID_TextBox.Text = context_.currLabTest_.Get_f_Id
        Ft7LabTest_PatientHospID_TextBox.Text = context_.currLabTest_.Get_f_Patient_Id ' forgein key of <-> Patient.f_id_

        Ft7LabTest_P1OrderDate_TextBox.Text = context_.currLabTest_.Get_f_P1_Date
        Ft7LabTest_P1Result_TextBox.Text = context_.currLabTest_.Get_f_P1_Result_Bin
        Ft7LabTest_P1Level_TextBox.Text = context_.currLabTest_.Get_f_P1_Result_Nominal
        Ft7LabTest_P1Root_TextBox.Text = context_.currLabTest_.Get_f_P1_Kind

        Ft7LabTest_P2OrderDate_TextBox.Text = context_.currLabTest_.Get_f_P2_Date
        Ft7LabTest_P2Result_TextBox.Text = context_.currLabTest_.Get_f_P2_Result_Bin
        Ft7LabTest_P2Level_TextBox.Text = context_.currLabTest_.Get_f_P2_Result_Nominal
        Ft7LabTest_P2Root_TextBox.Text = context_.currLabTest_.Get_f_P2_Kind

        context_.contextID_ = context_.currLabTest_.d_id_
    End Sub

    ' DUTY FORM <-> CONTEXT
    Sub ITabItem8Duty_UpdateContextDutyFromForm()
        Dim f_id As String = "" ' Predefined 
        Dim f_date As String = Ft8Duty_Date_DatePicker.Text
        Dim f_personel_id As String = Ft8Duty_PersonelHospID_TextBox.Text
        Dim has_duty As String = "1" ' Predefined

        context_.currDuty_.ValidateAndUpdate(f_id, f_date, f_personel_id, has_duty)
        context_.currDuty_.d_id_ = context_.contextID_
    End Sub
    Sub ITabItem8Duty_UpdateFormFromContextDuty()
        Ft8Duty_Date_DatePicker.Text = context_.currDuty_.Get_f_Date
        Ft8Duty_PersonelHospID_TextBox.Text = context_.currDuty_.Get_f_Personel_Id

        context_.contextID_ = context_.currDuty_.d_id_
    End Sub

    Sub ITabItem2Ward_Update()
        Log("Info: Aktaulzacja wpisu oddziału")
        ITabItem2Ward_UpdateContextWardFromForm()
        If context_.currWard_.IsInitialized Then
            Dim querry As String = context_.currWard_.FormQuerry_Update()
            Log("Info: Zaktualizowano informacje o oddziale " & context_.currWard_.Get_f_Name)
            Dim result = My.Application.DatabaseSqliteInterface_.ExecuteQuerry(querry)
            result.Clear()
            result.Dispose()
        Else
            Log("Błąd: Nie można zaktualizować danych oddziale - brak wybranego oddziału.")
        End If
    End Sub

    Sub ITabItem3Room_Update()
        Log("Info: Aktaulzacja wpisu sali")
        ITabItem3Room_UpdateContextRoomFromForm()
        If context_.currRoom_.IsInitialized Then
            Dim querry As String = context_.currRoom_.FormQuerry_Update()
            Log("Info: Zaktualizowano informacje o sali " & context_.currRoom_.Get_f_number)
            Dim result = My.Application.DatabaseSqliteInterface_.ExecuteQuerry(querry)
            result.Clear()
            result.Dispose()
        Else
            Log("Błąd: Nie można zaktualizować danych sali - brak wybranej sali.")
        End If
    End Sub

    Sub ITabItem4Personel_Update()
        Log("Info: Aktaulzacja wpisu personelu")
        ITabItem4Personel_UpdateContextHospitalStuffFromForm()
        If context_.currHospitalStaff_.IsInitialized Then
            Dim querry As String = context_.currHospitalStaff_.FormQuerry_Update()
            Log("Info: Zaktualizowano informacje o sali " & context_.currHospitalStaff_.Get_f_fullname)
            Dim result = My.Application.DatabaseSqliteInterface_.ExecuteQuerry(querry)
            result.Clear()
            result.Dispose()
        Else
            Log("Błąd: Nie można zaktualizować danych sali - brak wybranej sali.")
        End If
    End Sub


    Sub ITabItem5Patient_Update()
        Log("Info: Aktaulzacja wpisu pacjenta")
    End Sub

    Sub ITabItem6Visit_Update()
        Log("Info: Aktaulzacja wpisu wizyty")
        ITabItem6Visit_UpdateContextVisitFromForm()
        If context_.currVisit_.IsInitialized Then
            Dim querry As String = context_.currVisit_.FormQuerry_Update()
            Log("Info: Zaktualizowano informacje o wizycie " & context_.currVisit_.Get_f_Id)
            Dim result = My.Application.DatabaseSqliteInterface_.ExecuteQuerry(querry)
            result.Clear()
            result.Dispose()
        Else
            Log("Błąd: Nie można zaktualizować danych - brak wybranej wizyty.")
        End If
    End Sub

    Sub ITabItem7LabTest_Update()
        Log("Info: Aktaulzacja wpisu wizyty")
        ITabItem7LabTest_UpdateContextLabTestFromForm()
        If context_.currLabTest_.IsInitialized Then
            Dim querry As String = context_.currLabTest_.FormQuerry_Update()
            Log("Info: Zaktualizowano informacje o wizycie " & context_.currLabTest_.Get_f_Id)
            Dim result = My.Application.DatabaseSqliteInterface_.ExecuteQuerry(querry)
            result.Clear()
            result.Dispose()
        Else
            Log("Błąd: Nie można zaktualizować danych - brak wybranego badania.")
        End If
    End Sub

    Sub ITabItem8Duty_Update()
        Log("Info: Aktaulzacja wpisu dużyru")
        ITabItem8Duty_UpdateContextDutyFromForm()
        If context_.currDuty_.IsInitialized Then
            Dim querry As String = context_.currDuty_.FormQuerry_Update()
            Log("Info: Zaktualizowano informacje o dyżurze.")
            Dim result = My.Application.DatabaseSqliteInterface_.ExecuteQuerry(querry)
        Else
            Log("Błąd: Nie można zaktualizować danych - brak odpowiedniego dyżuru.")
        End If
    End Sub
    ' UPDATE FORM METHODS - 

    ' DELETE FORM METHODS - BEGIN
    Sub ITabItem2Ward_Delete()
        Log("ITabItem2Ward_Delete")
        ' Check the program context and if correct update data ID

        If Not context_.TabSelected_ = Context.TabSelected.Ward Or context_.contextID_ < 0 Then
            'Log("ITabItem2Ward_Delete: no data avaialbe to be deleted")
            Log("Błąd: Nie można wykonać operacji usunięcia oddziału - żaden nie został wybrany.")
            Return
        End If
        ' Prepare Delete querry
        ITabItem2Ward_UpdateContextWardFromForm()
        Dim deleteQuerry As String = context_.currWard_.FormQuerry_Delete()
        Dim result = My.Application.DatabaseSqliteInterface_.ExecuteQuerry(deleteQuerry)
        If Not result.HasErrors Then
            Log("Info: Usunięto informacje o oddziale z bazie danych. Odśwież widok by mieć aktualny.")
        End If
        result.Clear()
        result.Dispose()
    End Sub

    Sub ITabItem3Room_Delete()
        Log("ITabItem3Room_Delete")
        ' Check the program context and if correct update data ID

        If Not context_.TabSelected_ = Context.TabSelected.Room Or context_.contextID_ < 0 Then
            Log("Błąd: Nie można wykonać operacji usunięcia sali - żadna nie została wybrana.")
            Return
        End If
        ' Prepare Delete querry
        ITabItem3Room_UpdateContextRoomFromForm()

        Dim deleteQuerry As String = context_.currRoom_.FormQuerry_Delete()
        Dim result = My.Application.DatabaseSqliteInterface_.ExecuteQuerry(deleteQuerry)
        If Not result.HasErrors Then
            Log("Info: Usunięto informacje o sali z bazy danych. Odśwież widok by mieć aktualny.")
        End If
        result.Clear()
        result.Dispose()
    End Sub

    Sub ITabItem4Personel_Delete()
        Log("ITabItem4Personel_Delete")
        If Not context_.TabSelected_ = Context.TabSelected.Staff Or context_.contextID_ < 0 Then
            Log("Błąd: Nie można wykonać operacji usunięcia wpisu Pracownika - pozycja nie została wybrana.")
            Return
        End If
        ' Prepare Delete querry
        ITabItem4Personel_UpdateContextHospitalStuffFromForm()

        Dim deleteQuerry As String = context_.currHospitalStaff_.FormQuerry_Delete()
        Dim result = My.Application.DatabaseSqliteInterface_.ExecuteQuerry(deleteQuerry)
        If Not result.HasErrors Then
            Log("Info: Usunięto informacje o pracowniku z bazy danych. Odśwież widok by mieć aktualny.")
        End If
        result.Clear()
        result.Dispose()
    End Sub

    Sub ITabItem5Patient_Delete()
        Log("Usunięcie wpisu pacjenta")
    End Sub

    Sub ITabItem6Visit_Delete()
        Log("Usunięcie wizyty")
        If Not context_.TabSelected_ = Context.TabSelected.Visit Or context_.contextID_ < 0 Then
            Log("Błąd: Nie można wykonać operacji usunięcia wpisu wizyty - pozycja nie została wybrana.")
            Return
        End If
        ITabItem6Visit_UpdateContextVisitFromForm()
        Dim deleteQuerry As String = context_.currVisit_.FormQuerry_Delete()
        Dim result = My.Application.DatabaseSqliteInterface_.ExecuteQuerry(deleteQuerry)
        If Not result.HasErrors Then
            Log("Info: Usunięto informacje o wizycie z bazy danych. Odśwież widok by mieć aktualny.")
        End If
        result.Clear()
        result.Dispose()
    End Sub

    Sub ITabItem7LabTest_Delete()
        Log("ITabItem7LabTest_Delete")
        If Not context_.TabSelected_ = Context.TabSelected.LabTest Or context_.contextID_ < 0 Then
            Log("Błąd: Nie można wykonać operacji usunięcia badania - pozycja nie została wybrana.")
            Return
        End If
        'ITabItem7LabTest_UpdateContextLabTestFromForm() ' TODO Odkomentuj
        Dim deleteQuerry As String = context_.currLabTest_.FormQuerry_Delete()
        Dim result = My.Application.DatabaseSqliteInterface_.ExecuteQuerry(deleteQuerry)
        If Not result.HasErrors Then
            Log("Info: Usunięto informacje o badaniu z bazy danych. Odśwież widok by mieć aktualny.")
        End If
        result.Clear()
        result.Dispose()
    End Sub


    Sub ITabItem8Duty_Delete()
        Log("Usunięcie dyżuru")
        If Not context_.TabSelected_ = Context.TabSelected.Duty Or context_.contextID_ < 0 Then
            Log("Błąd: Nie można wykonać operacji usunięcia dyżuru - pozycja nie została wybrana.")
            Return
        End If
        ITabItem8Duty_UpdateContextDutyFromForm()
        Dim deleteQuerry As String = context_.currDuty_.FormQuerry_Delete()
        Dim result = My.Application.DatabaseSqliteInterface_.ExecuteQuerry(deleteQuerry)
        If Not result.HasErrors Then
            Log("Info: Usunięto informacje o dyżurze z bazy danych. Odśwież widok by mieć aktualny.")
        End If
        result.Clear()
        result.Dispose()
    End Sub

    Sub ITabItem2Ward_New()
        Log("Info: Dodanie nowego oddziału")
        ITabItem2Ward_UpdateContextWardFromForm()
        ' Jesli jest ok to pobierz wolny identyfikator.
        Dim next_free_d_id As Integer = My.Application.DatabaseSqliteInterface_.GetNextFreeIndexForTable(context_.currWard_.Get_TableName())
        If next_free_d_id < 0 Then
            Log("Błąd: Nie można znaleźć poprawnego ID - baza danych może być uszkodzona. Operacja anulowana.")
            Return
        End If
        If (context_.currWard_.Get_f_Id > -1) And (Not context_.currWard_.Get_f_Name = String.Empty) Then
            Dim insertQuerry As String = context_.currWard_.FormQuerry_Insert()
            Dim result = My.Application.DatabaseSqliteInterface_.ExecuteQuerry(insertQuerry)
            If Not result.HasErrors Then
                MsgBox("Info: Zapisano nowy oddział w bazie danych: " & context_.currWard_.Get_f_Name)
            End If
            result.Clear()
            result.Dispose()
        Else
            MsgBox("Błąd: Nie można utworzyć zapisu o nowym oddziale w bazie danych. ID Oddziału oraz jego nazwa muszą być zdefiniowane.")
        End If

    End Sub

    ' SINCE 2017-07-05
    Sub ITabItem3Room_New()
        Log("ITabItem3Room_New")
        ITabItem3Room_UpdateContextRoomFromForm()
        ' Jesli jest ok to pobierz wolny identyfikator.
        Dim next_free_d_id As Integer = My.Application.DatabaseSqliteInterface_.GetNextFreeIndexForTable(context_.currRoom_.Get_TableName())
        If next_free_d_id < 0 Then
            Log("Błąd: Nie można znaleźć poprawnego ID - baza danych może być uszkodzona. Operacja anulowana.")
            Return
        End If
        'Here and in next methdos: With Get_f_Id comparation explicit mismatch. TODO: Check this and fix!
        If (context_.currRoom_.Get_f_Id > -1) And (Not context_.currRoom_.Get_f_Type = String.Empty) Then
            Dim insertQuerry As String = context_.currRoom_.FormQuerry_Insert()
            Dim result = My.Application.DatabaseSqliteInterface_.ExecuteQuerry(insertQuerry)
            If Not result.HasErrors Then
                MsgBox("Info: Zapisano nową salę w bazie danych: " & context_.currRoom_.Get_f_number)
            End If
            result.Clear()
            result.Dispose()
        Else
            MsgBox("Błąd: Nie można utworzyć zapisu o nowej sali w bazie danych. ID Sali oraz jej typ muszą być zdefiniowane.")
        End If
    End Sub

    ' SINCE 2017-07-05
    Sub ITabItem4Personel_New()
        Log("ITabItem4Personel_New")

        ITabItem4Personel_UpdateContextHospitalStuffFromForm()
        ' Jesli jest ok to pobierz wolny identyfikator.
        Dim next_free_d_id As Integer = My.Application.DatabaseSqliteInterface_.GetNextFreeIndexForTable(context_.currHospitalStaff_.Get_TableName())
        If next_free_d_id < 0 Then
            Log("Błąd: Nie można znaleźć poprawnego ID - baza danych może być uszkodzona. Operacja anulowana.")
            Return
        End If
        If (context_.currHospitalStaff_.Get_f_Id > -1) And (Not context_.currHospitalStaff_.Get_f_Type = String.Empty) Then
            Dim insertQuerry As String = context_.currHospitalStaff_.FormQuerry_Insert()
            Dim result = My.Application.DatabaseSqliteInterface_.ExecuteQuerry(insertQuerry)
            If Not result.HasErrors Then
                MsgBox("Info: Zapisano nową pozycję Personel w bazie danych: " & context_.currHospitalStaff_.Get_f_fullname)
            End If
            result.Clear()
            result.Dispose()
        Else
            MsgBox("Błąd: Nie można utworzyć zapisu o nowej sali w bazie danych. ID Sali oraz jej typ muszą być zdefiniowane.")
        End If
    End Sub

    ' SINCE 2017-07-05
    Sub ITabItem6Visit_New()
        Log("Utworzenie nowej wizyty")
        ITabItem6Visit_UpdateContextVisitFromForm()
        ' Jesli jest ok to pobierz wolny identyfikator.
        Dim next_free_d_id As Integer = My.Application.DatabaseSqliteInterface_.GetNextFreeIndexForTable(context_.currVisit_.Get_TableName())
        If next_free_d_id < 0 Then
            Log("Błąd: Nie można znaleźć poprawnego ID - baza danych może być uszkodzona. Operacja anulowana.")
            Return
        End If
        If (context_.currVisit_.Get_f_Id > -1) And (Not context_.currVisit_.Get_f_Date_In = String.Empty) Then
            Dim insertQuerry As String = context_.currVisit_.FormQuerry_Insert()
            Dim result = My.Application.DatabaseSqliteInterface_.ExecuteQuerry(insertQuerry)
            If Not result.HasErrors Then
                MsgBox("Info: Zapisano nową wizytę w bazie danych: " & context_.currVisit_.Get_f_Id)
            End If
            result.Clear()
            result.Dispose()
        Else
            MsgBox("Błąd: Nie można utworzyć zapisu o nowej sali w bazie danych. ID Sali oraz jej typ muszą być zdefiniowane.")
        End If
    End Sub

    ' SINCE 2017-07-05
    Sub ITabItem7LabTest_New()
        Log("Utworzenie nowego wpisu o badaniu")
        ITabItem7LabTest_UpdateContextLabTestFromForm()
        ' Jesli jest ok to pobierz wolny identyfikator.
        Dim next_free_d_id As Integer = My.Application.DatabaseSqliteInterface_.GetNextFreeIndexForTable(context_.currLabTest_.Get_TableName())
        If next_free_d_id < 0 Then
            Log("Błąd: Nie można znaleźć poprawnego ID - baza danych może być uszkodzona. Operacja anulowana.")
            Return
        End If
        If (context_.currLabTest_.Get_f_Id > -1) And (Not context_.currLabTest_.Get_f_Patient_Id = String.Empty) Then
            Dim insertQuerry As String = context_.currLabTest_.FormQuerry_Insert()
            Dim result = My.Application.DatabaseSqliteInterface_.ExecuteQuerry(insertQuerry)
            If Not result.HasErrors Then
                MsgBox("Info: Zapisano nowe badanie w bazie danych: " & context_.currLabTest_.Get_f_Id)
            End If
            result.Clear()
            result.Dispose()
        Else
            MsgBox("Błąd: Nie można utworzyć zapisu o nowym badaniu w bazie danych. ID badania oraz pacjenta muszą być zdefiniowane.")
        End If
    End Sub

    Sub ITabItem8Duty_New()
        Log("Utworzenie nowego wpisu dużur")
        ITabItem8Duty_UpdateContextDutyFromForm()
        ' Jesli jest ok to pobierz wolny identyfikator.
        Dim next_free_d_id As Integer = My.Application.DatabaseSqliteInterface_.GetNextFreeIndexForTable(context_.currDuty_.Get_TableName())
        If next_free_d_id < 0 Then
            Log("Błąd: Nie można znaleźć poprawnego ID - baza danych może być uszkodzona. Operacja anulowana.")
            Return
        End If
        If (Not context_.currDuty_.Get_f_Date = String.Empty) And (Not context_.currDuty_.Get_f_Personel_Id = String.Empty) Then
            Dim insertQuerry As String = context_.currDuty_.FormQuerry_Insert()
            Dim result = My.Application.DatabaseSqliteInterface_.ExecuteQuerry(insertQuerry)
            If Not result.HasErrors Then
                MsgBox("Info: Zapisano nowy dyżur w bazie danych: data:" & context_.currDuty_.Get_f_Date & ", ID personelu " & context_.currDuty_.f_personel_id_)
            End If
            result.Clear()
            result.Dispose()
        Else
            MsgBox("Błąd: Nie można utworzyć zapisu o nowym dyżurze w bazie danych. Data oraz ID personelu muszą być zdefiniowane.")
        End If
    End Sub

    Private Sub MIF_Save_Click(sender As Object, e As RoutedEventArgs) Handles MIF_Save.Click
        My.Application.SaveConfigurationPersistently()
        Log("Zapisano ustawienia")
    End Sub

    Private Sub buttonMenExportData_Click(sender As Object, e As RoutedEventArgs) Handles buttonMenExportData.Click
        Log("Info: User eksportuje dane")
        ExportAllData()
    End Sub

    Sub ExportAllData()
        Log("Info: Eksportowanie danych...")
        Dim current_directory = Directory.GetCurrentDirectory()
        Dim location_of_exported_data = current_directory & "\" & My.Application.config_.SettingsDirectoryWhereExportedDataGoes_
        My.Application.dataExporter_.ExportAllTables(location_of_exported_data)
        Log("Info: Dane zostały wyeksportowane do katalogu: " & location_of_exported_data)
    End Sub

    Private Sub EntryFormWindow2_GotFocus(sender As Object, e As RoutedEventArgs) Handles EntryFormWindow2.GotFocus
        ' Disable Button 'New' for Tab Ward - freature requested 2017.07
        If TabItem2Ward.IsSelected() Then
            buttonNavNew.IsEnabled = False
        ElseIf TabItem3room.IsSelected() Then
            buttonNavNew.IsEnabled = False
        Else
            buttonNavNew.IsEnabled = True
        End If

    End Sub

End Class
