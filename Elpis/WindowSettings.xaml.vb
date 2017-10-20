Imports System.IO
Imports System.Windows.Forms

Public Class WindowSettings

    Dim maxIntervalNumber As Integer = 20160 ' Number of miuntes in two weeks: 20160

    Sub New()
        ' This call is required by the designer.
        InitializeComponent()
        ' Add any initialization after the InitializeComponent() call.

        SettingsSettingsDataBasePath_textBox.Text = My.Application.config_.SettingsDataBasePath_

        SettingsAutoImportDirectoryLocation_textBox.Text = My.Application.config_.SettingsAutoImportFilePathWithDataForImporting_
        SettingsAutoImportCheckBox.IsChecked = My.Application.config_.SettingsAutoImportEnabled_
        SettingsAutoImportUpdateTimeInterval_textBox.Text = My.Application.config_.SettingsAutoImportTimePeriodInMins_
        SettingsAutoImportHospitalName_textBox.Text = My.Application.config_.SettingsHospitalName_


    End Sub


    Private Sub SettingsOK_button_Click(sender As Object, e As RoutedEventArgs) Handles SettingsOK_button.Click

        My.Application.config_.SettingsHospitalName_ = SettingsAutoImportHospitalName_textBox.Text
        My.Application.config_.SettingsAutoImportEnabled_ = SettingsAutoImportCheckBox.IsChecked
        My.Application.config_.SettingsAutoImportFilePathWithDataForImporting_ = SettingsAutoImportDirectoryLocation_textBox.Text

        'DB settings:
        If Not My.Application.config_.SettingsDataBasePath_ = SettingsSettingsDataBasePath_textBox.Text Then
            If My.Application.DatabaseSqliteInterface_.ChangeDataBase(SettingsSettingsDataBasePath_textBox.Text) Then
                My.Application.config_.SettingsDataBasePath_ = SettingsSettingsDataBasePath_textBox.Text
                My.Application.config_.SettingsDataBaseIsInitialized_ = True
            Else
                MsgBox("Błąd: Utworzenie nowej bazy danych zakończyło się niepowodzeniem.")
            End If
        End If
        ' This will be not set here:
        'My.Application.config_.SettingsDataBaseIsInitialized_

        ' Validation 
        If IsNumeric(SettingsAutoImportUpdateTimeInterval_textBox.Text) Then
            Dim updateTimeInterval As Integer = 0
            Try
                updateTimeInterval = Convert.ToInt32(SettingsAutoImportUpdateTimeInterval_textBox.Text)
                If 0 < updateTimeInterval And updateTimeInterval < maxIntervalNumber Then
                    My.Application.config_.SettingsAutoImportTimePeriodInMins_ = updateTimeInterval

                    ' If Validation passed save settings 
                    My.Application.SaveConfigurationPersistently()

                    Close()
                Else
                    MsgBox("Validation error:  " & SettingsAutoImportUpdateTimeInterval_textBox.Text & " out of the allowed range (0, " & maxIntervalNumber & "). Change the number", MsgBoxStyle.Information)
                End If
            Catch ex As FormatException
                MsgBox("Validation error: " & SettingsAutoImportUpdateTimeInterval_textBox.Text & " has wrong format.", MsgBoxStyle.Information)
                SettingsAutoImportUpdateTimeInterval_textBox.Text = My.Application.config_.SettingsAutoImportTimePeriodInMins_
                updateTimeInterval = My.Application.config_.SettingsAutoImportTimePeriodInMins_
            Catch ex As OverflowException
                MsgBox("Validation error:  " & SettingsAutoImportUpdateTimeInterval_textBox.Text & " out of the allowed range (0, " & maxIntervalNumber & "). Change the number", MsgBoxStyle.Information)
                SettingsAutoImportUpdateTimeInterval_textBox.Text = My.Application.config_.SettingsAutoImportTimePeriodInMins_
                updateTimeInterval = My.Application.config_.SettingsAutoImportTimePeriodInMins_
            End Try
            Return
        Else
            MsgBox("Validation error: " & SettingsAutoImportUpdateTimeInterval_textBox.Text & " is not a numeric value. Provide proper parameter.", MsgBoxStyle.Information)
            Return
        End If
        Return
    End Sub

    Private Sub SettingsCancel_button_Click(sender As Object, e As RoutedEventArgs) Handles SettingsCancel_button.Click
        Close()
    End Sub

    Private Sub SettingsAutoImportDirectoryChange_button_Click(sender As Object, e As RoutedEventArgs) Handles SettingsAutoImportDirectoryChange_button.Click

        Dim myStream As Stream = Nothing
        Dim openFileDialog1 As New OpenFileDialog()

        'openFileDialog1.InitialDirectory = "c:\"
        openFileDialog1.InitialDirectory = IO.Path.GetFullPath(My.Application.Info.DirectoryPath)
        openFileDialog1.Filter = "txt files (*.csv)|*.csv|All files (*.*)|*.*"
        openFileDialog1.FilterIndex = 2
        openFileDialog1.RestoreDirectory = True

        If openFileDialog1.ShowDialog() = System.Windows.Forms.DialogResult.OK Then
            Try
                My.Application.config_.SettingsAutoImportFilePathWithDataForImporting_ = openFileDialog1.FileName

                SettingsAutoImportDirectoryLocation_textBox.Text = openFileDialog1.FileName
                'myStream = openFileDialog1.OpenFile()
                If (myStream IsNot Nothing) Then
                    ' Insert code to read the stream here.

                End If
            Catch Ex As Exception
                MessageBox.Show("Cannot read file from disk. Original error: " & Ex.Message)
            Finally
                ' Check this again, since we need to make sure we didn't throw an exception on open.
                If (myStream IsNot Nothing) Then
                    myStream.Close()
                End If
            End Try
        End If

    End Sub


    Private Sub SettingsSettingsDataBasePathChange_button_Click(sender As Object, e As RoutedEventArgs) Handles SettingsSettingsDataBasePathChange_button.Click

        Dim myStream As Stream = Nothing
        Dim openFileDialog2 As New OpenFileDialog()

        openFileDialog2.InitialDirectory = IO.Path.GetFullPath(My.Application.Info.DirectoryPath)
        openFileDialog2.Filter = "txt files (*.db3)|*.db3|All files (*.*)|*.*"
        openFileDialog2.FilterIndex = 2
        openFileDialog2.RestoreDirectory = True

        If openFileDialog2.ShowDialog() = System.Windows.Forms.DialogResult.OK Then
            Try
                My.Application.config_.SettingsAutoImportFilePathWithDataForImporting_ = openFileDialog2.FileName

                SettingsSettingsDataBasePath_textBox.Text = openFileDialog2.FileName
                'myStream = openFileDialog1.OpenFile()
                If (myStream IsNot Nothing) Then
                    ' Insert code to read the stream here.

                End If
            Catch Ex As Exception
                MessageBox.Show("Cannot read db3 file from disk. Original error: " & Ex.Message)
            Finally
                ' Check this again, since we need to make sure we didn't throw an exception on open.
                If (myStream IsNot Nothing) Then
                    myStream.Close()
                End If
            End Try
        End If

    End Sub

    Private Sub SettingsAutoImportCheckBox_Checked(sender As Object, e As RoutedEventArgs) Handles SettingsAutoImportCheckBox.Click
        If Not SettingsAutoImportCheckBox.IsChecked Then
            SettingsAutoImportUpdateTimeInterval_textBox.IsEnabled = False
            SettingsAutoImportDirectoryLocation_textBox.IsEnabled = False
        Else
            SettingsAutoImportUpdateTimeInterval_textBox.IsEnabled = True
            SettingsAutoImportDirectoryLocation_textBox.IsEnabled = True
        End If

    End Sub
End Class
