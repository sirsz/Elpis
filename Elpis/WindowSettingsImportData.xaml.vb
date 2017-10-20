Imports System.IO
Imports System.Windows.Forms

Public Class WindowSettingsImportData

    Sub New()

        ' This call is required by the designer.
        InitializeComponent()
        ' Add any initialization after the InitializeComponent() call.

        IDWard_checkBox.IsChecked = My.Application.config_.SettingsInitData_Ward_Loaded_
        IDRoom_checkBox.IsChecked = My.Application.config_.SettingsInitData_Room_Loaded_
        IDStaff_checkBox.IsChecked = My.Application.config_.SettingsInitData_Staff_Loaded_

        IDStaffDoctAttr_checkBox.IsChecked = My.Application.config_.SettingsInitData_StaffDoctorAttr_Loaded_
        IDStaffNurseAttr_checkBox.IsChecked = My.Application.config_.SettingsInitData_StaffNurseAttr_Loaded_
        IDStructure_checkBox.IsChecked = My.Application.config_.SettingsInitData_Structure_Loaded_
        IDTopology_checkBox.IsChecked = My.Application.config_.SettingsInitData_Topology_Loaded_

        IDWard_textBox.Text = My.Application.config_.SettingsInitData_Ward_PathToFile_
        IDRoom_textBox.Text = My.Application.config_.SettingsInitData_Room_PathToFile_
        IDStaff_textBox.Text = My.Application.config_.SettingsInitData_Staff_PathToFile_

        IDStaffDoctAttr_textBox.Text = My.Application.config_.SettingsInitData_StaffDoctorAttr_PathToFile_
        IDStaffNurseAttr_textBox.Text = My.Application.config_.SettingsInitData_StaffNurseAttr_PathToFile_
        IDStructure_textBox.Text = My.Application.config_.SettingsInitData_Structure_PathToFile_
        IDTopology_textBox.Text = My.Application.config_.SettingsInitData_Topology_PathToFile_

    End Sub

    Private Sub UpdateConfig()

        My.Application.config_.SettingsInitData_Ward_Loaded_ = IDWard_checkBox.IsChecked
        My.Application.config_.SettingsInitData_Room_Loaded_ = IDRoom_checkBox.IsChecked
        My.Application.config_.SettingsInitData_Staff_Loaded_ = IDStaff_checkBox.IsChecked

        My.Application.config_.SettingsInitData_StaffDoctorAttr_Loaded_ = IDStaffDoctAttr_checkBox.IsChecked
        My.Application.config_.SettingsInitData_StaffNurseAttr_Loaded_ = IDStaffNurseAttr_checkBox.IsChecked
        My.Application.config_.SettingsInitData_Structure_Loaded_ = IDStructure_checkBox.IsChecked
        My.Application.config_.SettingsInitData_Topology_Loaded_ = IDTopology_checkBox.IsChecked

        My.Application.config_.SettingsInitData_Ward_PathToFile_ = IDWard_textBox.Text
        My.Application.config_.SettingsInitData_Room_PathToFile_ = IDRoom_textBox.Text
        My.Application.config_.SettingsInitData_Staff_PathToFile_ = IDStaff_textBox.Text

        My.Application.config_.SettingsInitData_StaffDoctorAttr_PathToFile_ = IDStaffDoctAttr_textBox.Text
        My.Application.config_.SettingsInitData_StaffNurseAttr_PathToFile_ = IDStaffNurseAttr_textBox.Text
        My.Application.config_.SettingsInitData_Structure_PathToFile_ = IDStructure_textBox.Text
        My.Application.config_.SettingsInitData_Topology_PathToFile_ = IDTopology_textBox.Text

        ' If Validation passed save settings 
        My.Application.SaveConfigurationPersistently()

    End Sub

    Private Sub ID_CloseWindow_button_Click(sender As Object, e As RoutedEventArgs) Handles ID_CloseWindow_button.Click
        UpdateConfig()
        Close()
    End Sub

    Private Function SelectCsvFile(ByRef pathToCsvFile As String) As Boolean
        SelectCsvFile = False
        Dim openFileDialog1 As New OpenFileDialog()
        'openFileDialog1.InitialDirectory = IO.Path.GetFullPath(My.Application.Info.DirectoryPath)
        openFileDialog1.Filter = "txt files (*.csv)|*.csv|All files (*.*)|*.*"
        openFileDialog1.FilterIndex = 2
        openFileDialog1.RestoreDirectory = True
        If openFileDialog1.ShowDialog() = System.Windows.Forms.DialogResult.OK Then
            Try

                pathToCsvFile = openFileDialog1.FileName
                SelectCsvFile = True
            Catch Ex As Exception
                MessageBox.Show("[Elpis:] Nie można otworzyć pliku, szczegóły błędu: " & Ex.Message)
                SelectCsvFile = False
            End Try
        End If
        ' On Else: do nothing
    End Function



    Private Sub IDWard_buttonSelect_Click(sender As Object, e As RoutedEventArgs) Handles IDWard_buttonSelect.Click
        Dim pathToCsvFile As String = String.Empty
        Dim selectingFileResult As Boolean = False
        selectingFileResult = SelectCsvFile(pathToCsvFile)
        If selectingFileResult Then
            IDWard_textBox.Text = pathToCsvFile
            UpdateConfig()
        End If
    End Sub

    Private Sub IDRoom_buttonSelect_Click(sender As Object, e As RoutedEventArgs) Handles IDRoom_buttonSelect.Click
        Dim pathToCsvFile As String = String.Empty
        Dim selectingFileResult As Boolean = False
        selectingFileResult = SelectCsvFile(pathToCsvFile)
        If selectingFileResult Then
            IDRoom_textBox.Text = pathToCsvFile
            UpdateConfig()
        End If
    End Sub

    Private Sub IDStaff_buttonSelect_Click(sender As Object, e As RoutedEventArgs) Handles IDStaff_buttonSelect.Click
        Dim pathToCsvFile As String = String.Empty
        Dim selectingFileResult As Boolean = False
        selectingFileResult = SelectCsvFile(pathToCsvFile)
        If selectingFileResult Then
            IDStaff_textBox.Text = pathToCsvFile
            UpdateConfig()
        End If
    End Sub

    Private Sub IDStaffDoctAttr_buttonSelect_Click(sender As Object, e As RoutedEventArgs) Handles IDStaffDoctAttr_buttonSelect.Click
        Dim pathToCsvFile As String = String.Empty
        Dim selectingFileResult As Boolean = False
        selectingFileResult = SelectCsvFile(pathToCsvFile)
        If selectingFileResult Then
            IDStaffDoctAttr_textBox.Text = pathToCsvFile
            UpdateConfig()
        End If
    End Sub

    Private Sub IDStaffNurseAttr_buttonSelect_Click(sender As Object, e As RoutedEventArgs) Handles IDStaffNurseAttr_buttonSelect.Click
        Dim pathToCsvFile As String = String.Empty
        Dim selectingFileResult As Boolean = False
        selectingFileResult = SelectCsvFile(pathToCsvFile)
        If selectingFileResult Then
            IDStaffNurseAttr_textBox.Text = pathToCsvFile
            UpdateConfig()
        End If
    End Sub

    Private Sub IDStructure_buttonSelect_Click(sender As Object, e As RoutedEventArgs) Handles IDStructure_buttonSelect.Click
        Dim pathToCsvFile As String = String.Empty
        Dim selectingFileResult As Boolean = False
        selectingFileResult = SelectCsvFile(pathToCsvFile)
        If selectingFileResult Then
            IDStructure_textBox.Text = pathToCsvFile
            UpdateConfig()
        End If
    End Sub

    Private Sub IDTopology_buttonSelect_Click(sender As Object, e As RoutedEventArgs) Handles IDTopology_buttonSelect.Click
        Dim pathToCsvFile As String = String.Empty
        Dim selectingFileResult As Boolean = False
        selectingFileResult = SelectCsvFile(pathToCsvFile)
        If selectingFileResult Then
            IDTopology_textBox.Text = pathToCsvFile
            UpdateConfig()
        End If
    End Sub

    Private Sub IDWard_buttonLoad_Click(sender As Object, e As RoutedEventArgs) Handles IDWard_buttonLoad.Click
        My.Application.dataLoaderModule_.LoadFromFile_Wards(My.Application.config_.SettingsInitData_Ward_PathToFile_)
        IDWard_checkBox.IsChecked = True
    End Sub

    Private Sub IDRoom_buttonLoad_Click(sender As Object, e As RoutedEventArgs) Handles IDRoom_buttonLoad.Click
        My.Application.dataLoaderModule_.LoadFromFile_Rooms(My.Application.config_.SettingsInitData_Room_PathToFile_)
        IDRoom_checkBox.IsChecked = True
    End Sub
    Private Sub IDStaff_buttonLoad_Click(sender As Object, e As RoutedEventArgs) Handles IDStaff_buttonLoad.Click
        My.Application.dataLoaderModule_.LoadFromFile_Staff(My.Application.config_.SettingsInitData_Staff_PathToFile_)
        IDStaff_checkBox.IsChecked = True
    End Sub

    Private Sub IDStaffDoctAttr_buttonLoad_Click(sender As Object, e As RoutedEventArgs) Handles IDStaffDoctAttr_buttonLoad.Click
        My.Application.dataLoaderModule_.LoadFromFile_StaffDoctorAttr(My.Application.config_.SettingsInitData_StaffDoctorAttr_PathToFile_)
        IDStaffDoctAttr_checkBox.IsChecked = True
    End Sub

    Private Sub IDStaffNurseAttr_buttonLoad_Click(sender As Object, e As RoutedEventArgs) Handles IDStaffNurseAttr_buttonLoad.Click
        My.Application.dataLoaderModule_.LoadFromFile_StaffNurseAttr(My.Application.config_.SettingsInitData_StaffNurseAttr_PathToFile_)
        IDStaffNurseAttr_checkBox.IsChecked = True
    End Sub

    Private Sub IDStructure_buttonLoad_Click(sender As Object, e As RoutedEventArgs) Handles IDStructure_buttonLoad.Click
        My.Application.dataLoaderModule_.LoadFromFile_Structure(My.Application.config_.SettingsInitData_Structure_PathToFile_)
        IDStructure_checkBox.IsChecked = True
    End Sub

    Private Sub IDTopology_buttonLoad_Click(sender As Object, e As RoutedEventArgs) Handles IDTopology_buttonLoad.Click
        My.Application.dataLoaderModule_.LoadFromFile_Topology(My.Application.config_.SettingsInitData_Topology_PathToFile_)
        IDTopology_checkBox.IsChecked = True
    End Sub


End Class
