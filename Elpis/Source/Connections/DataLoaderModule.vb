Imports System.IO

Public Class DataLoaderModule

    Private csvItemsSeparator_ As String = ";"

    Public Enum LoadingCategory
        UNSET
        LOADING_WARD
        LOADING_ROOM
        LOADING_PERSONEL
    End Enum

    Public Sub New()
        'Console.WriteLine(DateTime.Now.ToString("[yyyy/MM/dd HH:mm:ss]: ") & "DataReaderModule has been created")
        Toolbox.Log("[Elpis Moduł czytania danych inicjalizacyjnych] Moduł został utworzony.")
    End Sub


    Private Sub CopyFileTo(ByVal sourcePath As String, ByVal destinationDirectory As String)
        If Not Directory.Exists(destinationDirectory) Then
            Directory.CreateDirectory(destinationDirectory)
        End If
        Dim file = New FileInfo(sourcePath)
        file.CopyTo(Path.Combine(destinationDirectory, file.Name), True)
    End Sub


    Sub InsertNewWard(
        ByVal WardHospId As String,
        ByVal WardName As String,
        ByVal WardType As String,
        ByVal WardBedNumberMax As String,
        ByVal WardBedNumberMean As String)

        Dim ward_temp As New Ward
        ward_temp.ValidateAndUpdate(WardHospId, WardName, WardType, WardBedNumberMax, WardBedNumberMean)
        Dim insertQuerry As String = ward_temp.FormQuerry_Insert()
        Dim result = My.Application.DatabaseSqliteInterface_.ExecuteQuerry(insertQuerry)
        If result.HasErrors Then
            Toolbox.Log("Info: Zapisanie nowej pozycji oddział (ID:" & WardHospId & ") w bazie danych nie powiodło się.")
        End If
        result.Clear()
        result.Dispose()

    End Sub

    Sub InsertNewRoom(
        ByVal id As String,
        ByVal number As String,
        ByVal type As String,
        ByVal bed_number_max As String,
        ByVal bed_number_mean As String,
        ByVal ward_id As String)

        Dim room_temp As New Room
        room_temp.ValidateAndUpdate(id, number, type, bed_number_max, bed_number_mean, ward_id)
        Dim insertQuerry As String = room_temp.FormQuerry_Insert()
        Dim result = My.Application.DatabaseSqliteInterface_.ExecuteQuerry(insertQuerry)
        If result.HasErrors Then
            Toolbox.Log("Info: Zapisanie nowej sali (ID:" & id & ") w bazie danych nie powiodło się.")
        End If
        result.Clear()
        result.Dispose()
    End Sub

    Sub InsertNewPersonel(
        ByVal id As String,
        ByVal fullName As String,
        ByVal service_kind As String)

        Dim hospital_staff_temp As New HospitalStaff
        hospital_staff_temp.ValidateAndUpdate(id, fullName, service_kind)
        Dim insertQuerry As String = hospital_staff_temp.FormQuerry_Insert()
        Dim result = My.Application.DatabaseSqliteInterface_.ExecuteQuerry(insertQuerry)
        If result.HasErrors Then
            Toolbox.Log("Info: Zapisanie nowej pozycji personel (ID:" & id & ") w bazie danych nie powiodło się.")
        End If
        result.Clear()
        result.Dispose()
    End Sub

    '==================================

    Private Function ImportNewDataFromFile(ByRef path As String, ByVal loadingCategory As LoadingCategory) As Boolean

        ImportNewDataFromFile = False
        Dim loadedItems As Integer = 0
        Using MyReader As New Microsoft.VisualBasic.FileIO.TextFieldParser(path)
            MyReader.TextFieldType = FileIO.FieldType.Delimited
            MyReader.SetDelimiters(";")
            Dim currentRow As String()
            While Not MyReader.EndOfData
                Try
                    currentRow = MyReader.ReadFields()
                    Select Case loadingCategory
                        Case LoadingCategory.LOADING_WARD
                            If currentRow.Count = 5 Or currentRow.Count = 6 Then ' Wards must have 5 elements
                                'Here is defined order of the CSV file: 
                                'Wczytaj oddziały: Ward.csv
                                'ID_WARD;NAME;TYPE;MAX_BED_NUMBER;MEN_BED_OCCUP; DESCR
                                Dim wardHospId As String = currentRow.ElementAt(0)
                                Dim wardName As String = currentRow.ElementAt(1)
                                Dim wardType As String = currentRow.ElementAt(2)
                                Dim wardBedNumberMax As String = currentRow.ElementAt(3)
                                Dim wardBedNumberMean As String = currentRow.ElementAt(4)
                                'Dim wardDescr As String = currentRow.ElementAt(5) - Description is not used any more. Uncomment in case of restoring that information.

                                InsertNewWard(wardHospId, wardName, wardType, wardBedNumberMax, wardBedNumberMean)
                                loadedItems += 1
                            End If
                        Case LoadingCategory.LOADING_ROOM
                            If currentRow.Count = 5 Then ' Room must have 5 elements while should six.
                                'Here is defined order of the CSV file:
                                'Wczytaj sale: Room.csv
                                'ID_ROOM;ID_WARD;TYPE; MAX_BED_NUMBER;MEN_BED_OCCUP
                                Dim roomId As String = currentRow.ElementAt(0)
                                Dim wardIdWhereTheRoomIs As String = currentRow.ElementAt(1)
                                Dim roomType As String = currentRow.ElementAt(2)
                                Dim roomBedNumberMax As String = currentRow.ElementAt(3)
                                Dim roomBedNumberMean As String = currentRow.ElementAt(4)
                                Dim roomNumber As String = String.Empty ' Not given in csv file but needed for InsertNewRoom method.

                                InsertNewRoom(roomId, roomNumber, roomType, roomBedNumberMax, roomBedNumberMean, wardIdWhereTheRoomIs)
                                loadedItems += 1
                            End If
                        Case LoadingCategory.LOADING_PERSONEL
                            If currentRow.Count = 3 Then ' Room must have 5 elements while should six.
                                'Wczytaj personel: staf.csv
                                'ID_STAF;TYPE;NAME
                                Dim staffId As String = currentRow.ElementAt(0)
                                Dim staffType As String = currentRow.ElementAt(1)
                                Dim staffFullName As String = currentRow.ElementAt(2)

                                InsertNewPersonel(staffId, staffFullName, staffType)
                                loadedItems += 1
                            End If

                        Case Else
                            Toolbox.Log("Error: Undefined category: " & loadingCategory)

                    End Select
                Catch ex As Microsoft.VisualBasic.
                            FileIO.MalformedLineException
                    'Console.WriteLine("Line " & ex.Message & "is not valid and will be skipped.")
                    Toolbox.Log("[Elpis autoimport wizyt] Wystąpił błąd podczas importowania danych. Wpis został ominięty. Szczegóły błędu: " & ex.Message)
                End Try
            End While
        End Using
        Toolbox.Log("[Elpis import] Wczytano " & loadedItems & " pozycji.")
    End Function

    '==================================

    Public Function LoadFromFile_Wards(ByRef path As String) As Boolean
        LoadFromFile_Wards = False
        If System.IO.File.Exists(path) Then
            Toolbox.Log("[Elpis Moduł czytania danych inicjalizacyjnych] Wczytywanie oddziałów z pliku " & path)
            ImportNewDataFromFile(path, LoadingCategory.LOADING_WARD)
        End If
    End Function


    Public Function LoadFromFile_Rooms(ByRef path As String) As Boolean
        LoadFromFile_Rooms = False
        If System.IO.File.Exists(path) Then
            Toolbox.Log("[Elpis Moduł czytania danych inicjalizacyjnych] Wczytywanie sal z pliku " & path)
            ImportNewDataFromFile(path, LoadingCategory.LOADING_ROOM)
        End If
    End Function

    Public Function LoadFromFile_Staff(ByRef path As String) As Boolean
        LoadFromFile_Staff = False
        If System.IO.File.Exists(path) Then
            Toolbox.Log("[Elpis Moduł czytania danych inicjalizacyjnych] Wczytywanie personelu z pliku " & path)
            ImportNewDataFromFile(path, LoadingCategory.LOADING_PERSONEL)
        End If
    End Function

    Public Function LoadFromFile_StaffDoctorAttr(ByRef path As String) As Boolean
        LoadFromFile_StaffDoctorAttr = False
        If System.IO.File.Exists(path) Then
            CopyFileTo(path, My.Application.config_.SettingsDirectoryWhereExportedDataGoes_)
            Toolbox.Log("[Elpis Moduł czytania danych inicjalizacyjnych] Wczytywanie przypisania personelu medycznego z pliku " & path)
        End If
    End Function

    Public Function LoadFromFile_StaffNurseAttr(ByRef path As String) As Boolean
        LoadFromFile_StaffNurseAttr = False
        If System.IO.File.Exists(path) Then
            Toolbox.Log("[Elpis Moduł czytania danych inicjalizacyjnych] Wczytywanie przypisania personelu pielęgniarskiego z pliku " & path)
            CopyFileTo(path, My.Application.config_.SettingsDirectoryWhereExportedDataGoes_)
            LoadFromFile_StaffNurseAttr = True
        End If
    End Function

    Public Function LoadFromFile_Structure(ByRef path As String) As Boolean
        LoadFromFile_Structure = False
        If System.IO.File.Exists(path) Then
            Toolbox.Log("[Elpis Moduł czytania danych inicjalizacyjnych] Wczytywanie struktury z pliku " & path)
            CopyFileTo(path, My.Application.config_.SettingsDirectoryWhereExportedDataGoes_)
            LoadFromFile_Structure = True
        End If
    End Function

    Public Function LoadFromFile_Topology(ByRef path As String) As Boolean
        LoadFromFile_Topology = False
        If System.IO.File.Exists(path) Then
            Toolbox.Log("[Elpis Moduł czytania danych inicjalizacyjnych] Wczytywanie topologii z pliku " & path)
            CopyFileTo(path, My.Application.config_.SettingsDirectoryWhereExportedDataGoes_)
            LoadFromFile_Topology = True
        End If
    End Function

End Class
