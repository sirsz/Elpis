Public Class Hospital

    Public id_ As UInteger
    Public name_ As String
    Public address_ As String
    Public description_ As String

    Private wardList_ As New List(Of Ward)()

    Sub New(ByVal id As UInteger, ByVal name As String)
        id_ = id
        name_ = name
    End Sub
    Public Sub AddWard(ByRef ward)
        wardList_.Add(ward)
    End Sub

    Public Sub ListWard()
        For Each ward In wardList_
            Console.WriteLine(ward)
        Next
    End Sub

End Class
