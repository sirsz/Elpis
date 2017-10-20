Imports System.Collections.ObjectModel


Public Class AnalysisEngine


    Dim items_to_show_ As Integer = 12
    Dim delimeter_ As String = ","

    Public Sub New()
        'Console.WriteLine(DateTime.Now.ToString("[yyyy/MM/dd HH:mm:ss]: ") & "DataReaderModule has been created")
        Toolbox.Log("[Elpis moduł analizy] został utworzony.")
    End Sub


    Public Function ImportNewDataFromFile(ByRef report_file_path As String, ByRef observable_collection As ObservableCollection(Of KeyValuePair(Of String, Int32))) As Boolean
        ' Adapted: https://docs.microsoft.com/en-us/dotnet/articles/visual-basic/developing-apps/programming/drives-directories-files/how-to-read-from-comma-delimited-text-files
        ImportNewDataFromFile = False
        Dim dictionary As ObservableCollection(Of KeyValuePair(Of String, Int32)) = New ObservableCollection(Of KeyValuePair(Of String, Int32))
        Dim row_index As Integer = 0
        Dim rows_accepted As Integer = 0

        If Not My.Computer.FileSystem.FileExists(report_file_path) Then
            Toolbox.Log("[Elpis moduł analizy: Błąd:] Plik z raportem nie został znaleziony: Oczekiwana ścieżka: " & report_file_path)
            Return False
        End If

        Using MyReader As New Microsoft.VisualBasic.FileIO.TextFieldParser(report_file_path)
            MyReader.TextFieldType = FileIO.FieldType.Delimited
            MyReader.SetDelimiters(delimeter_)
            Dim currentRow As String()

            While Not MyReader.EndOfData
                currentRow = MyReader.ReadFields()
                If row_index > 0 Then ' Skip the first header line.
                    Try
                        If currentRow.Count = 2 Then ' Visits must have 5 elements
                            'Here is defined order of the CSV file:
                            Dim PatientHospID As String = currentRow.ElementAt(0)
                            Dim RiskLevel_text As String = currentRow.ElementAt(1)
                            Dim RiskLevel_val As Integer = -1
                            If RiskLevel_text = String.Empty Then
                                RiskLevel_val = -1.0
                                ' Log that wrong format found in report
                                Dim err_msg_row As String = String.Empty
                                For Each currentField In currentRow
                                    err_msg_row += " " & currentField
                                Next
                                Toolbox.Log("[Elpis moduł analizy: Błąd:] Niepoprawne dane w raporcie: " & err_msg_row)
                            Else
                                RiskLevel_val = CInt(RiskLevel_text)
                                dictionary.Add(New KeyValuePair(Of String, Integer)(PatientHospID, RiskLevel_val))
                                rows_accepted += 1
                                ImportNewDataFromFile = True
                            End If
                        End If
                    Catch ex As Microsoft.VisualBasic.FileIO.MalformedLineException
                        Toolbox.Log("[Elpis moduł analizy: Błąd:] Wystąpił błąd podczas czytania raportu. Szczegóły błędu: " & ex.Message)
                    End Try
                End If ' row_index > 0
                row_index += 1
            End While
        End Using

        ' Sort dictionary here 

        If rows_accepted - 1 < items_to_show_ Then
            items_to_show_ = row_index - 1
        End If

        If dictionary.Count < items_to_show_ Then
            items_to_show_ = dictionary.Count - 1
        End If

        If items_to_show_ > 0 Then
            For item As Integer = 0 To items_to_show_
                observable_collection.Add(dictionary.ElementAt(item))
            Next
        End If

    End Function



End Class
