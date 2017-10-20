' Source: https://social.msdn.microsoft.com/Forums/vstudio/en-US/4c0c8946-d163-49b6-a957-b94caffa69ea/draw-column-chart-in-wpf-using-text-box-values?forum=wpf

Imports System.Collections.ObjectModel

Public Class WindowReport3_ref

    Dim dictionary As ObservableCollection(Of KeyValuePair(Of Int32, Int32)) = New ObservableCollection(Of KeyValuePair(Of Int32, Int32))
    Public Sub New()

        ' This call is required by the designer.
        InitializeComponent()

        ' Add any initialization after the InitializeComponent() call.
        columnSeries.ItemsSource = dictionary

    End Sub
    Private Sub Button_Click(sender As Object, e As RoutedEventArgs)
        'The TextBoxes must contain valid integer values when the button is clicked for this sample code to work!
        dictionary.Add(New KeyValuePair(Of Integer, Integer)(Convert.ToInt32(rad.Text), Convert.ToInt32(thita.Text)))
    End Sub
End Class
