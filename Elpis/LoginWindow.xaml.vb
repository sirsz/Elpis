Class LoginWindow

    Private loginStr_ As String = "user1"
    Private passwordStr_ As String = "23451"

    Private mainWindow As New ElpisMainWindow(My.Application.context_)

    Private Sub LoginConfirmButton_Click(sender As Object, e As RoutedEventArgs) Handles LoginConfirmButton.Click

        Dim loginStr As String
        Dim passwordStr As String
        loginStr = LoginUserTextBox.Text
        passwordStr = LoginPasswordPasswordBox.Password
        If loginStr = loginStr_ Then
            If loginStr = loginStr_ And passwordStr = passwordStr_ Then

                My.Application.authenticated_ = True
                mainWindow.Show()
                Me.Close()

            Else
                MsgBox("Autentykacja nieudana: hasło niepoprawne.")
            End If
        Else
            MsgBox("Autentykacja nieudana: login nieznany.")
        End If
    End Sub
End Class
