Imports System.Collections.ObjectModel

Public Class WindowReport3
    Dim dictionary As ObservableCollection(Of KeyValuePair(Of String, Int32)) = New ObservableCollection(Of KeyValuePair(Of String, Int32))

    Dim dataLoaderEngine_ As New AnalysisEngine

    Dim path_to_report As String = "siralgorytm\result\report-visits.csv"

    Public Sub New()

        ' This call is required by the designer.
        InitializeComponent()
        FillDictionary()
        ' Add any initialization after the InitializeComponent() call.
        columnSeries.ItemsSource = dictionary


    End Sub

    Private Sub FillDictionary_for_testing()
        'The TextBoxes must contain valid integer values when the button is clicked for this sample code to work!
        dictionary.Add(New KeyValuePair(Of String, Integer)("12523", 10341))
        dictionary.Add(New KeyValuePair(Of String, Integer)("63432", 7523))
        dictionary.Add(New KeyValuePair(Of String, Integer)("36324", 4234))
        dictionary.Add(New KeyValuePair(Of String, Integer)("12414", 4161))
        dictionary.Add(New KeyValuePair(Of String, Integer)("74565", 2341))
        dictionary.Add(New KeyValuePair(Of String, Integer)("23426", 1341))
        dictionary.Add(New KeyValuePair(Of String, Integer)("13426", 241))

    End Sub


    Private Sub FillDictionary()
        dataLoaderEngine_.ImportNewDataFromFile(path_to_report, dictionary)
    End Sub

    'Private Sub Button_Click(sender As Object, e As RoutedEventArgs)
    '    'The TextBoxes must contain valid integer values when the button is clicked for this sample code to work!
    '    dictionary.Add(New KeyValuePair(Of String, Integer)(visit.Text, Convert.ToInt32(risk.Text)))
    'End Sub
End Class
