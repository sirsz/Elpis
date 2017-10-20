
Imports System.Web.UI.DataVisualization.Charting
Imports OxyPlot
Imports OxyPlot.Series

Public Class WindowReport2
    Private mmodel As PlotModel

    Public Class Record
        Dim _id, _age As Integer
        Dim _name As String
        Public Sub New(ByVal id As Integer, ByVal name As String, ByVal age As Integer)
            Me._id = id
            Me._name = name
            Me._age = age
        End Sub

        Public Property ID() As Integer
            Get
                Return _id
            End Get
            Set(ByVal Value As Integer)
                _id = Value
            End Set
        End Property

        Public Property Name() As String
            Get
                Return _name
            End Get
            Set(ByVal Value As String)
                _name = Value
            End Set
        End Property

        Public Property Age() As Integer
            Get
                Return _age
            End Get
            Set(ByVal Value As Integer)
                _age = Value
            End Set
        End Property
    End Class

    Private Sub New()
        ' Create a list. 
        Dim listDataSource As New ArrayList()

        ' Populate the list with records. 
        listDataSource.Add(New Record(1, "Jane", 19))
        listDataSource.Add(New Record(2, "Joe", 30))
        listDataSource.Add(New Record(3, "Bill", 15))
        listDataSource.Add(New Record(4, "Michael", 42))

        ' Bind the chart to the list. 
        'Dim myChart As ChartControl = chartControl1
        'myChart.DataSource = listDataSource


        'ColumnChart1.DataContext = listDataSource

    End Sub

    'Public Sub New()

    '    Model = New PlotModel()

    '    Model.Title = "Simple example"
    '    Model.Subtitle = "using OxyPlot in VB.NET"

    '    Dim series1 = New LineSeries()
    '    series1.Title = "Series 1"
    '    series1.MarkerType = MarkerType.Circle
    '    series1.Points.Add(New DataPoint(0, 0))
    '    series1.Points.Add(New DataPoint(10, 18))
    '    series1.Points.Add(New DataPoint(20, 12))
    '    series1.Points.Add(New DataPoint(30, 8))
    '    series1.Points.Add(New DataPoint(40, 15))

    '    Dim series2 = New LineSeries()
    '    series2.Title = "Series 2"
    '    series2.MarkerType = MarkerType.Square
    '    series2.Points.Add(New DataPoint(0, 4))
    '    series2.Points.Add(New DataPoint(10, 12))
    '    series2.Points.Add(New DataPoint(20, 16))
    '    series2.Points.Add(New DataPoint(30, 25))
    '    series2.Points.Add(New DataPoint(40, 5))

    '    Model.Series.Add(series1)
    '    Model.Series.Add(series2)

    '    Dim series3 = New KeyValuePair(Of String, Integer)

    '    Dim list As List(Of KeyValuePair(Of String, Integer)) = New List(Of KeyValuePair(Of String, Integer))

    '    list.Add(New KeyValuePair(Of String, Integer)("dot", 1))
    '    list.Add(New KeyValuePair(Of String, Integer)("net", 2))
    '    list.Add(New KeyValuePair(Of String, Integer)("perls", 3))


    '    Dim dd As DataVisualization.Charting.Series

    '    'ColumnChart1.Series.Add("Series2")
    '    'ColumnChart1.Series.Add("Series2").Points.AddY(20)

    'End Sub

    'Sub ShowChart2()

    '    Dim list2 As List(Of KeyValuePair(Of String, Integer)) = New List(Of KeyValuePair(Of String, Integer))

    '    list2.Add(New KeyValuePair(Of String, Integer)("dot", 1))
    '    list2.Add(New KeyValuePair(Of String, Integer)("net", 2))
    '    list2.Add(New KeyValuePair(Of String, Integer)("perls", 3))

    '    ColumnChart1.DataContext = list2
    'End Sub


    'Property Model() As PlotModel
    '    Get
    '        Return mmodel
    '    End Get
    '    Set(value As PlotModel)
    '        mmodel = value
    '    End Set
    ''End Property

    'Private Sub ShowChart_Click(sender As Object, e As RoutedEventArgs) Handles ShowChart.Click
    '    ShowChart2()
    'End Sub
End Class
