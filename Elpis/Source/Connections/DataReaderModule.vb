Imports System.Threading

Public Class DataReaderModule


    Public Sub New()
        'Console.WriteLine(DateTime.Now.ToString("[yyyy/MM/dd HH:mm:ss]: ") & "DataReaderModule has been created")
        Toolbox.Log("[Elpis autoimport wizyt] Moduł został utworzony.")
    End Sub

    Private mstrLine As String

    'Property DataReaderModule() As String
    '    Get
    '        Return mstrLine
    '    End Get
    '    Set(ByVal Value As String)
    '        mstrLine = Value
    '    End Set
    'End Property

    'ReadOnly Property Length() As Integer
    '    Get
    '        Return mstrLine.Length
    '    End Get
    'End Property
    Public Sub LaunchMonitor()

        trd = New Thread(AddressOf ThreadTask)
        trd.IsBackground = True
        trd.Start()

    End Sub

    Private Function NewDataToImportAvailable(ByRef path As String) As Boolean
        NewDataToImportAvailable = False
        If System.IO.File.Exists(path) Then
            Dim lastUpdateTime As Date = System.IO.File.GetLastWriteTime(path)
            If lastUpdateTime > lastUpdateTime_ Then
                'Console.WriteLine("New data for import available: " & lastUpdateTime & ", while last imported on: " & lastUpdateTime_)
                Toolbox.Log("[Elpis autoimport wizyt] Nowe dane są dostępne (plik został zaktualizowany: " & lastUpdateTime & "), ostatni import danych: " & lastUpdateTime_)
                lastUpdateTime_ = lastUpdateTime
                NewDataToImportAvailable = True
            End If
        End If
    End Function

    Private Sub ThreadTask()

        Dim iteration As Integer = 0
        Do
            Console.WriteLine("Input data update (check " & iteration & ")")
            If My.Application.config_.SettingsAutoImportEnabled_ Then
                If NewDataToImportAvailable(My.Application.config_.SettingsAutoImportFilePathWithDataForImporting_) Then
                    ImportNewDataFromFile(My.Application.config_.SettingsAutoImportFilePathWithDataForImporting_)
                End If
            End If
            Dim sleepTime = My.Application.config_.SettingsAutoImportTimePeriodInMins_ * 60000 ' there is 60'000 (60*1000) miliseconds in one minute
            Thread.Sleep(sleepTime)
            iteration += 1
        Loop
    End Sub

    Private Function ImportNewDataFromFile(ByRef path As String) As Boolean
        ' Adapted: https://docs.microsoft.com/en-us/dotnet/articles/visual-basic/developing-apps/programming/drives-directories-files/how-to-read-from-comma-delimited-text-files
        ImportNewDataFromFile = False
        Dim loadedItems As Integer = 0
        Using MyReader As New Microsoft.VisualBasic.FileIO.TextFieldParser(path)
            MyReader.TextFieldType = FileIO.FieldType.Delimited
            MyReader.SetDelimiters(";")
            Dim currentRow As String()
            While Not MyReader.EndOfData
                Try
                    currentRow = MyReader.ReadFields()
                    'Dim currentField As String
                    'Console.WriteLine(" - New line loaded:")
                    ' Debuging:
                    'For Each currentField In currentRow
                    '    Console.WriteLine("  - current field:  """ & currentField & """")
                    'Next
                    If currentRow.Count = 5 Then ' Visits must have 5 elements
                        'Here is defined order of the CSV file:
                        'CH.REQ#1106: Dim VisitHospitalId As String = currentRow.ElementAt(0)
                        Dim VisitPatientName As String = currentRow.ElementAt(0)
                        Dim WardId As String = currentRow.ElementAt(1)
                        Dim SigninDate As String = currentRow.ElementAt(2)
                        Dim SignoutDate As String = currentRow.ElementAt(3)
                        InsertNewVisit(VisitPatientName, SigninDate, SignoutDate, WardId)
                        loadedItems += 1
                    End If
                Catch ex As Microsoft.VisualBasic.
                            FileIO.MalformedLineException
                    'Console.WriteLine("Line " & ex.Message & "is not valid and will be skipped.")
                    Toolbox.Log("[Elpis autoimport wizyt] Wystąpił błąd podczas importowania danych. Wpis został ominięty. Szczegóły błędu: " & ex.Message)
                End Try
            End While
        End Using
        Toolbox.Log("[Elpis autoimport wizyt] Wczytano " & loadedItems & " pozycji.")
    End Function


    Sub InsertNewVisit(
        ByRef VisitPatientName As String,
        ByRef SigninDate As String,
        ByRef SignoutDate As String,
        ByRef WardId As String)

        Dim visit_temp As New Visit
        If visit_temp.ValidateAndUpdate(VisitPatientName, SigninDate, SignoutDate, WardId) Then
            Dim insertQuerry As String = visit_temp.FormQuerry_Insert()
            Dim result = My.Application.DatabaseSqliteInterface_.ExecuteQuerry(insertQuerry)
            If result.HasErrors Then
                Toolbox.Log("Info: Nie można było zapisać nowej wizyty (dla:" & VisitPatientName & ") w bazie danych.")
                'Else
                '    Toolbox.Log("Info: Zapisano nową wizytę w bazie danych z ID: " & VisitHospitalId)
            End If
            result.Clear()
            result.Dispose()
        Else
            Toolbox.Log("Info: Nie można było sparsować nowej wizyty (dla:" & VisitPatientName & "). Wpis ten nie zostanie zaimportowany")
        End If
    End Sub


    Private trd As Thread
    Private lastUpdateTime_ As Date ' TODO: How to securly initialize date ? My local format "1970-01-01 00:00:00 AM"


End Class
