Public Class ProductSales
    Public Sub New(ByVal quantity As Integer, ByVal month As Integer, ByVal monthName As String)
        Me.Quantity = quantity
        Me.Month = month
        Me.MonthName = monthName
    End Sub
    Private _Quantity As Integer
    Public Property Quantity() As Integer
        Get
            Return _Quantity
        End Get
        Set(ByVal value As Integer)
            _Quantity = value
        End Set
    End Property
    Private _Month As Integer
    Public Property Month() As Integer
        Get
            Return _Month
        End Get
        Set(ByVal value As Integer)
            _Month = value
        End Set
    End Property
    Private _MonthName As String
    Public Property MonthName() As String
        Get
            Return _MonthName
        End Get
        Set(ByVal value As String)
            _MonthName = value
        End Set
    End Property
End Class