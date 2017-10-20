Public Class Patient

    Public id_ As Integer = 0
    Public firstName_ As String = ""
    Public lastName_ As String = ""
    Public birthDate_ As Date
    Public hospitalId_ As Integer = 0
    Public peselId_ As String = ""

    Sub New()
        firstName_ = "Unknown name"
        lastName_ = "Unknown name"
    End Sub

    Sub New(ByVal firstName As String, ByVal lastName As String)
        firstName_ = firstName
        lastName_ = lastName
    End Sub
    Sub New(ByVal firstName As String, ByVal lastName As String, ByVal birthDate As Date)
        firstName_ = firstName
        lastName_ = lastName
        birthDate_ = birthDate
    End Sub

End Class
