Public Class Toolbox

    Private Shared logFileName_ As String = "ElpisRuntime.log"
    Private Shared isOpened_ As Boolean
    Private Shared logFile_ As IO.StreamWriter


    Public Shared Function SplitString(ByVal s As String) As String()
        Dim parts As String() = s.Split(New Char() {"|"c})
        Return parts
    End Function


    Public Shared Sub Log(ByVal newLog As String)
        LogToConsole(newLog)
        LogToFile(newLog)
    End Sub

    Public Shared Sub LogToConsole(ByVal newLog As String)
        Console.WriteLine(DateTime.Now.ToString("[yyyy/MM/dd HH:mm:ss]: ") & newLog)
    End Sub

    Public Shared Sub LogToFile(ByVal newLog As String)
        If logFile_ Is Nothing Then
            logFile_ = My.Computer.FileSystem.OpenTextFileWriter(logFileName_, True)
        End If
        logFile_.WriteLine(DateTime.Now.ToString("[yyyy/MM/dd HH:mm:ss]: ") & newLog)
    End Sub

    Protected Overrides Sub Finalize()
        If Not logFile_ Is Nothing Then
            logFile_.Close()
        End If
    End Sub

End Class


