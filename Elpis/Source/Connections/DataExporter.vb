Imports System.Data
Imports System
Imports System.IO
Imports System.Data.SqlClient

Public Class DataExporter

    Dim Database_sqlite_interface_ As DbSqliteInterface
    Dim items_separator_in_file_ As Char = ";"
    Dim directory_name_ As String = "exported_data\"

    Dim VisitHeader_ As String = String.Empty

    ' Constructor
    Public Sub New(ByRef Database_sqlite_interface As DbSqliteInterface)
        Database_sqlite_interface_ = Database_sqlite_interface
    End Sub

    ' Write data table to file
    Sub WriteDataTableToFile(ByRef dt As DataTable, ByVal file_name As String)

        Try
            'create a stream object which can write text to a file
            Dim outputStream As StreamWriter = New StreamWriter(file_name, False)
            Dim item_with_separator As String = ""
            If dt.Rows.Count > 0 Then

                ' Write Visit header if defined (implementation only for Visits)
                If VisitHeader_ IsNot String.Empty Then
                    outputStream.Write(VisitHeader_)
                    outputStream.WriteLine()
                End If

                ' Write The received table.
                For Each row As DataRow In dt.Rows
                    For Each cell_obj As Object In row.ItemArray
                        If Not cell_obj Is Nothing Then
                            item_with_separator = String.Empty
                            item_with_separator = cell_obj.ToString & items_separator_in_file_
                            outputStream.Write(item_with_separator)
                        End If
                    Next
                    outputStream.WriteLine()
                Next
            End If
            outputStream.Close()
        Catch ex As Exception
            MessageBox.Show(ex.Message, "Export to file failed")
        End Try
    End Sub

    Function GetSelectQueryForTable(ByVal table_name) As String

        Dim search_query As String = String.Empty
        VisitHeader_ = String.Empty

        Dim context_method_provider As New Context
        Select Case table_name
            Case context_method_provider.currWard_.Get_TableName
                context_method_provider.currWard_.Clear()
                search_query = context_method_provider.currWard_.FormQuerry_Select
            Case context_method_provider.currRoom_.Get_TableName
                context_method_provider.currRoom_.Clear()
                search_query = context_method_provider.currRoom_.FormQuerry_Select
            Case context_method_provider.currHospitalStaff_.Get_TableName
                context_method_provider.currHospitalStaff_.Clear()
                search_query = context_method_provider.currHospitalStaff_.FormQuerry_Select
            Case context_method_provider.currVisit_.Get_TableName
                context_method_provider.currVisit_.Clear()
                VisitHeader_ = context_method_provider.currVisit_.FormQuerry_SelectSpecialForExport_Header
                search_query = context_method_provider.currVisit_.FormQuerry_SelectSpecialForExport
            Case context_method_provider.currLabTest_.Get_TableName
                context_method_provider.currLabTest_.Clear()
                search_query = context_method_provider.currLabTest_.FormQuerry_Select
            Case context_method_provider.currDuty_.Get_TableName
                context_method_provider.currDuty_.Clear()
                VisitHeader_ = context_method_provider.currDuty_.FormQuerry_SelectSpecialForExport_Header
                search_query = context_method_provider.currDuty_.FormQuerry_SelectSpecialForExport
            Case Else
                search_query = ""
                Toolbox.Log("[DataExporter Error:] Can not find query for the table " & table_name)
        End Select
        Return search_query
    End Function


    Public Sub ExportTableToFile(ByVal table_name As String, ByVal file_name As String)
        Dim query = GetSelectQueryForTable(table_name)
        Dim table = Database_sqlite_interface_.ExecuteQuerry(query)
        WriteDataTableToFile(table, file_name)
        table.Clear()
        table.Dispose()
    End Sub


    Public Sub ExportAllTables(Optional ByVal target_directory As String = "")

        ' Use the target_directory location if given
        If Not target_directory = String.Empty Then
            directory_name_ = target_directory
        End If

        ' Create directory if not exitsts:
        If (Not System.IO.Directory.Exists(directory_name_)) Then
            System.IO.Directory.CreateDirectory(directory_name_)
        End If
        ' Add backslash if is not present at the end of the the directory path:
        If Not directory_name_.Last = "\" Then
            directory_name_ += "\"
        End If

        Dim context_method_provider As New Context
        ExportTableToFile(context_method_provider.currWard_.Get_TableName, directory_name_ & context_method_provider.currWard_.Get_TableName.ToLower() & ".csv")
        ExportTableToFile(context_method_provider.currRoom_.Get_TableName, directory_name_ & context_method_provider.currRoom_.Get_TableName.ToLower() & ".csv")
        ExportTableToFile(context_method_provider.currHospitalStaff_.Get_TableName, directory_name_ & context_method_provider.currHospitalStaff_.Get_TableName.ToLower() & ".csv")
        ExportTableToFile(context_method_provider.currVisit_.Get_TableName, directory_name_ & context_method_provider.currVisit_.Get_TableName.ToLower() & ".csv")
        ExportTableToFile(context_method_provider.currLabTest_.Get_TableName, directory_name_ & context_method_provider.currLabTest_.Get_TableName.ToLower() & ".csv")
        ExportTableToFile(context_method_provider.currDuty_.Get_TableName, directory_name_ & context_method_provider.currDuty_.Get_TableName.ToLower() & ".csv")
    End Sub

End Class
