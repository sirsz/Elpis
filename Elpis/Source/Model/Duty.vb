

Imports System.Collections.Generic

Public Enum DutyType
    Undefined
    Lekarska
    Pielęgniarska
    Other
End Enum


Public Class Duty

    ' Formating:
    ' f_ - form value
    ' d_ data base value
    ' other 

    'Public id_ As UInteger
    Public d_id_ As Integer
    Public f_id_ As Integer
    Public f_date_ As Date
    Public f_personel_id_ As Integer ' forgein key of <-> HospitalStaff.f_id_ 
    Public f_has_duty_ As Boolean ' Optional value

    Private table_name_ As String = "DUTY"
    Private timeFormat_ As String = "yyyy-MM-dd"

    Function IsInitialized() As Boolean
        Return d_id_ > -1
    End Function

    Function Get_d_Id() As String
        Return d_id_.ToString
    End Function

    Function Get_f_Id() As String
        Return f_id_.ToString
    End Function

    Function Get_f_Date() As String
        Return f_date_.ToString(timeFormat_)
    End Function

    Function Get_f_Personel_Id() As String
        Return f_personel_id_.ToString
    End Function

    Function Get_Has_Duty() As String
        Return f_has_duty_.ToString
    End Function

    Function Get_TableName() As String
        Return table_name_
    End Function

    Private cQuote As String = Chr(34)

    Function ValidateAndUpdate(
        ByVal f_id As String,
        ByVal f_date As String,
        ByVal f_personel_id As String,
        Optional ByVal f_has_duty As String = ""
    ) As Boolean
        Dim f_id_local As Integer = -1
        Dim f_date_local As Date
        Dim f_personel_id_local As Integer = -1
        Dim f_has_duty_local As Boolean = False
        Dim validation_passed = True
        Try
            If f_id = String.Empty Then
                f_id_local = -1
                d_id_ = -1 ' IF Ward id was not set then the data shall be treated not initialized
            Else
                f_id_local = CInt(f_id)
            End If

            If f_date = String.Empty Then
                f_date_local = Date.MinValue
            Else
                If Not Date.TryParse(f_date, f_date_local) Then
                    f_date_local = Date.MinValue
                End If
            End If

            If f_personel_id = String.Empty Then
                f_personel_id_local = -1
            Else
                f_personel_id_local = CInt(f_personel_id)
            End If

            If (f_has_duty = "1" Or f_has_duty = "True" Or f_has_duty = "true") Then
                f_has_duty_local = True
            End If

        Catch ex As Exception
            validation_passed = False
            MsgBox("Hospital staff validation failed - wrong data " & ex.Message, MsgBoxStyle.Information)
            'Toolbox.Log("Ward validation failed - wrong data " & ex.Message, MsgBoxStyle.Information)
        End Try
        If validation_passed Then
            Update(f_id_local, f_date_local, f_personel_id_local, f_has_duty_local)
        End If
        Return validation_passed
    End Function


    Sub Update(
        ByVal f_id As Integer,
        ByVal f_date As Date,
        ByVal f_personel_id As Integer,
        ByVal f_has_duty As Boolean
    )
        f_id_ = f_id
        f_date_ = f_date
        f_personel_id_ = f_personel_id
        f_has_duty_ = f_has_duty
    End Sub


    Sub Clear() ' Reset class fields 
        f_id_ = -1
        f_date_ = Date.MinValue
        f_personel_id_ = -1
        f_has_duty_ = False
    End Sub


    ' This Parse method and FormQuerry_Select must keep the same columns order
    Function Parse(ByVal s As String) As Boolean
        Dim d_id_local As String
        Dim f_id_local As String
        Dim f_date_local As String
        Dim f_personel_id_local As String
        Dim f_has_duty_local As String = "0"

        Dim parts As String() = Toolbox.SplitString(s)
        d_id_local = parts(0)
        f_id_local = parts(1)
        f_date_local = parts(2)
        f_personel_id_local = parts(3)
        f_has_duty_local = parts(4)
        If Not f_has_duty_local = "1" Then ' Only "1" in database is accepted as "true" value.
            f_has_duty_local = "0"
        End If
        ValidateAndUpdate(f_id_local, f_date_local, f_personel_id_local, f_has_duty_local)
        d_id_ = CInt(d_id_local)

    End Function

    ' This FormQuerry_Select method and Parse must keep the same columns order
    Function FormQuerry_Select() As String

        ' THE DUTY QUERRY ORDER: |                 0                 |        1         |      2        |        3         |    4     |
        ' THE DUTY QUERRY ORDER: |                ID                 |    ID_DUTY       |    DATE       |    STAFF_ID      | HAS_DUTY |
        ' THE DUTY QUERRY ORDER: | INTEGER PRIMARY KEY AUTOINCREMENT | INTEGER NOT NULL | DATE NOT NULL | INTEGER NOT NULL |  BOOLEAN | 

        Dim querry As String = String.Empty
        Dim date_condition As String = String.Empty
        Dim personel_condition As String = String.Empty
        querry = "SELECT ID, ID_DUTY, DATE, STAFF_ID, HAS_DUTY FROM DUTY"

        If f_date_ > Date.MinValue Then
            date_condition = " DATE = " + cQuote + f_date_.ToString(timeFormat_) + cQuote
        End If
        If f_personel_id_ > -1 Then
            personel_condition = " STAFF_ID = " + f_personel_id_.ToString()
        End If
        ' Search filtering over Duty Date and/or Personel ID. Use these fields for filtering only when defined.
        If f_date_ > Date.MinValue Or f_personel_id_ > -1 Then
            querry += " WHERE "
            If f_date_ > Date.MinValue And f_personel_id_ > -1 Then
                querry += date_condition + " AND " + personel_condition
            End If
            If f_date_ > Date.MinValue And f_personel_id_ = -1 Then
                querry += date_condition
            End If
            If f_date_ = Date.MinValue And f_personel_id_ > -1 Then
                querry += personel_condition
            End If
        End If

        querry += ";"
        Return querry
    End Function

    ' This FormQuerry_Select method and Parse must keep the same columns order
    ' Defined in email 2017-07-06: 
    ' data_przyjecia, data_wypisu, id_pacjenta, id_oddzialu
    Function FormQuerry_SelectSpecialForExport() As String

        ' THE DUTY QUERRY ORDER: |                 0                 |        2         |      2        |
        ' THE DUTY QUERRY ORDER: |                ID                 |    STAFF_ID      |    DATE       |
        ' THE DUTY QUERRY ORDER: | INTEGER PRIMARY KEY AUTOINCREMENT | INTEGER NOT NULL | DATE NOT NULL |

        Dim querry As String = String.Empty
        querry = "SELECT ID, STAFF_ID, STRFTIME('%Y-%m-%d',DATE) FROM DUTY"
        querry += ";"
        Return querry
    End Function

    ' This FormQuerry_Select method and Parse must keep the same columns order
    ' Defined in email 2017-09-19: 
    ' (id_personelu, data) + and data base id.
    Function FormQuerry_SelectSpecialForExport_Header() As String
        Dim header As String = String.Empty
        header = "id_wpisu;id_personelu;data_dyżuru;"
        Return header
    End Function

    Public Function FormQuerry_Insert() As String
        Dim querry As String = String.Empty
        Dim has_duty_sql_string = "0"
        If f_has_duty_ = True Then
            has_duty_sql_string = "1"
        End If
        querry = "INSERT INTO DUTY (ID_DUTY, DATE, STAFF_ID, HAS_DUTY) VALUES (" +
            f_id_.ToString & "," &
            cQuote & f_date_.ToString(timeFormat_) & cQuote & "," &
            f_personel_id_.ToString & "," &
            has_duty_sql_string &
            ");"
        Return querry

    End Function

    Function FormQuerry_Update() As String ' Same as Update

        Dim querry As String = String.Empty
        ' Utworzenie querry

        ' TODO: Complete that
        ' "Type=" & quote & DutyType.ToString() & quote & ", " &
        Dim has_duty_sql_string = "0"
        If f_has_duty_ = True Then
            has_duty_sql_string = "1"
        End If
        querry = "UPDATE DUTY SET " &
         " ID_DUTY = " & f_id_.ToString() & ", " &
         " DATE = " & cQuote & f_date_.ToString(timeFormat_) & cQuote & ", " &
         " STAFF_ID = " & f_personel_id_.ToString() & ", " &
         " HAS_DUTY = " & has_duty_sql_string & " " &
         " WHERE ID = " & +d_id_.ToString &
         ";"
        Return querry
    End Function

    Function FormQuerry_Delete() As String
        Dim querry As String = String.Empty
        querry = "DELETE FROM DUTY " &
        " WHERE ID = " & +d_id_.ToString & ";"
        Return querry
    End Function


    Private Shared Function FormQuerry_DropTableIfExists() As String
        Dim querry As String = String.Empty
        querry = "DROP TABLE IF EXISTS DUTY; "
        Return querry
    End Function

    Private Shared Function FormQuerry_CreateTableIfNotExists() As String
        Dim querry As String = String.Empty
        querry = "CREATE TABLE IF NOT EXISTS DUTY (  
                    ID INTEGER PRIMARY KEY AUTOINCREMENT,
                    ID_DUTY INTEGER NOT NULL,
                    DATE DATE NOT NULL,
                    STAFF_ID INTEGER NOT NULL,
                    HAS_DUTY BOOLEAN
                  );"
        Return querry
    End Function

    Public Shared Function FormQuerry_DropAndCreateTable() As String
        Dim querry As String = String.Empty
        querry = FormQuerry_DropTableIfExists()
        querry += FormQuerry_CreateTableIfNotExists()
        Return querry
    End Function

End Class

