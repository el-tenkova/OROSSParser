Attribute VB_Name = "ConvertROS"
Sub ConvertROS()
   
    Dim fst As Object
    Set fst = CreateObject("ADODB.Stream")
    fst.Type = 2
    fst.Charset = "utf-8"
    fst.Open
    Set theDoc = ActiveDocument
    Dim para As Paragraph
    Set para = theDoc.Paragraphs.Item(1)
    cp = theDoc.Paragraphs.count
    
    fst.WriteText ("INSERT INTO `orfo`.`changes` (`id`, `key_article`, `title`, `text`, `src`, `dic`, `action`, `status`, `chd`, `username`) VALUES ")
    For i = 1 To cp
        Dim str As String
        If Not para Is Nothing Then
            If para.Range.Characters.count > 1 Then
                str = GetROSArticle(para)
                If i > 1 Then
                    fst.WriteText (",")
                End If
                fst.WriteText (" (NULL")
                fst.WriteText (", " & "'" & "255" & "', '', ")
                fst.WriteText ("'" & str & "'")
                fst.WriteText (", '', '49', '4', '1', CURRENT_DATE(), 'Иванова О.Е.')")
            End If
        Set para = para.Next()
        Else
            Exit For
        End If
    Next i
    fst.WriteText (";")
    NameFST = ActiveDocument.Path & "\" & Mid$(ActiveDocument.name, 1, InStrRev(ActiveDocument.name, ".")) & "sql"
    fst.SaveToFile NameFST, 2
    fst.Close
End Sub
Function GetROSArticle(para As Paragraph) As String
    wc = para.Range.words.count
    Dim Article As String
    Article = ""
    If wc > 2 Then
        Article = ConvertText(para.Range.words, 1, 0)
    End If
    GetROSArticle = Article
End Function
Function DoReplacements(text As String) As String

    text = Replace(text, "<b><i>" & ChrW$(&HD) & ChrW$(&H7) & "</i></b>", "")
    text = Replace(text, "<b><i>" & ChrW$(&HD) & "</i></b>", "")
    text = Replace(text, "<i><u>" & ChrW$(&HD) & ChrW$(&H7) & "</u></i>", "")
    text = Replace(text, "<i><u>" & ChrW$(&HD) & "</u></i>", "")
    
    text = Replace(text, "<i>" & ChrW$(&HD) & ChrW$(&H7) & "</i>", "")
    text = Replace(text, "<i>" & ChrW$(&HD) & "</i>", "")
    
    text = Replace(text, "<b>" & ChrW$(&HD) & ChrW$(&H7) & "</b>", "")
    text = Replace(text, "<b>" & ChrW$(&HD) & "</b>", "")
    
    text = Replace(text, "<u>" & ChrW$(&HD) & ChrW$(&H7) & "</u>", "")
    text = Replace(text, "<u>" & ChrW$(&HD) & "</u>", "")
    text = Replace(text, "<i>(", "(<i>")
    text = Replace(text, ")</i>", "</i>)")
    text = Replace(text, "(<b>", "<b>(")
    text = Replace(text, "</b>)", ")</b>")

    text = Replace(text, ChrW(&HA0), " ")
    text = Replace(text, vbCrLf, "")
    text = Replace(text, ChrW(&HD), "")
    text = Replace(text, ChrW(&H7), "")
    text = Replace(text, ChrW(&H1A), "-")
    text = Replace(text, ChrW(&H1E), "-")
    text = Replace(text, ChrW(&H2D), "-")
    text = Replace(text, ChrW(&HAD), "-")
'    text = Replace(text, ChrW$(&H7), "")
    For i = 1 To 3
        ch = "b"
        If i = 2 Then
            ch = "i"
        End If
        If i = 3 Then
            ch = "u"
        End If
        ' remove </b><b>, </i><i>, </u><u>
        a = "</" & ch & "><" & ch & ">"
        text = Replace(text, a, "")
        ' do not mark non-alphabetic
        '[
        text = Replace(text, "</" & ch & ">[<" & ch & ">", "[")
        ']
        text = Replace(text, "</" & ch & ">]<" & ch & ">", "]")
        '(
        text = Replace(text, "</" & ch & ">(<" & ch & ">", "(")
        ')
        text = Replace(text, "</" & ch & ">)<" & ch & ">", ")")
        
        '<b>[ => [<b>
        '<b>] => ]<b>
        text = Replace(text, "<" & ch & ">[", "[<" & ch & ">")
        text = Replace(text, "<" & ch & ">]", "]<" & ch & ">")
        ']</b> => </b>]
        '[</b> => </b>[
        text = Replace(text, "]</" & ch & ">", "</" & ch & ">]")
        text = Replace(text, "[</" & ch & ">", "</" & ch & ">[")
        '<b>( => (<b>
        ')<b> => <b>)
        'text = Replace(text, "<" & ch & ">(", "(<" & ch & ">")
        'text = Replace(text, ")<" & ch & ">", "<" & ch & ">)")
        '<b>, => ,<b>
        text = Replace(text, "<" & ch & ">,", ",<" & ch & ">")
        ',</b> => </b>,
        text = Replace(text, ",</" & ch & ">", "</" & ch & ">,")
        '<b>: => :<b>
        text = Replace(text, "<" & ch & ">:", ":<" & ch & ">")
        ',</b> => </b>,
        text = Replace(text, ":</" & ch & ">", "</" & ch & ">:")
        '/
        text = Replace(text, "</" & ch & ">/<" & ch & ">", "/")
        '-
        text = Replace(text, "</" & ch & ">-<" & ch & ">", "-")
        ']/[
        text = Replace(text, "</" & ch & ">]/[<" & ch & ">", "]/[")
    
    Next i
    text = Replace(text, "))", ")")
    text = Replace(text, "((", "(")
    text = Trim$(text)
        
    DoReplacements = text
End Function

Function ConvertText(words As words, start As Integer, finish As Integer, Optional noAccent As Boolean = False, Optional noFootNote As Boolean = False) As String
    wc = words.count
    Dim length1 As Integer
    Dim length2 As Integer
    Dim text As String
    Dim Code As Integer
    text = ""
    If finish = 0 Then
        finish = wc
    End If
    For j = start To finish
        If Len(words(j).text) > 0 Then
'            If words(j).Footnotes.count > 0 And Not noFootNote Then
'                text = text & "<sup><a href=" & Chr(34) & "#foot" & Trim$(str$(footNote)) & Chr(34)
'                text = text & " id=" & Chr(34) & "ft" & Trim$(str$(footNote)) & Chr(34) & " >"
'                text = text & "[" & Trim$(str$(footNote)) & "]</a></sup>"
'                footNote = footNote + 1
 '           Else
                If words.Item(j).Characters.count >= 1 And words.Item(j).Characters.Item(1) <> "§" Then
                    If words.Item(j).Bold Then
                        text = text + "<b>"
                    End If
                    If words.Item(j).Italic Then
                        text = text + "<i>"
                    End If
                End If
    '            If para.Range.Words.item(j).Underline Then
                    'Debug.Print words.item(j).text
                    For i = 1 To words.Item(j).Characters.count
'                        Debug.Print AscW(words.item(j).Characters(i).text)
                        If words.Item(j).Characters(i).text <> " " Then
                            'ch = AscW(words.item(j).Characters(i).text)
                            'Debug.Print ch
                            If words(j).Characters(i).Footnotes.count > 0 And Not noFootNote Then
                                text = text & "<sup><a href=" & Chr(34) & "#foot" & Trim$(str$(footNote)) & Chr(34)
                                text = text & " id=" & Chr(34) & "ft" & Trim$(str$(footNote)) & Chr(34) & " >"
                                text = text & "[" & Trim$(str$(footNote)) & "]</a></sup>"
                                footNote = footNote + 1
                            Else
                                If words.Item(j).Characters(i) = "#" Then
                                    text = text + "&#x301;"
                                Else
                                    If words.Item(j).Characters(i).Font.Superscript <> 0 Then
                                        text = text + "<sup>" + words.Item(j).Characters(i).text + "</sup>"
                                    Else
                                        If words.Item(j).Characters(i).Font.Subscript <> 0 Then
                                            text = text + "<sub>" + words.Item(j).Characters(i).text + "</sub>"
                                        Else
                                            If words.Item(j).Characters(i).Underline Then
                                                text = text + "<u>" ' + words.Item(j).Characters(i).text
                                                'If Not noAccent And words.Item(j).Characters(i).Font.name = "Times Roman Cyr Acsent" Then
                                                '    text = text + "&#x301"
                                                'End If
                                                If words.Item(j).Characters(i).Font.Hidden = 0 And words.Item(j).Characters(i).Font.Color <> wdColorWhite Then
                                                    Code = AscW(Mid(words.Item(j).Characters(i).text, 1, 1))
                                                    text = text + Mid(words.Item(j).Characters(i).text, 1, 1)
                                                    If Not noAccent And words.Item(j).Characters(i).Font.name = "Times Roman Cyr Acsent" Then
                                                        If AccentIsPossible(Code) = True Then
                                                            text = text + "&#x301;"
                                                        End If
                                                    Else
                                                        If Not noAccent And Len(words.Item(j).Characters(i).text) > 1 Then
                                                            '0x301
                                                            Code = AscW(Mid(words.Item(j).Characters(i).text, 2, 1))
                                                            If Code = &H301 Then
                                                                Code = AscW(Mid(words.Item(j).Characters(i).text, 1, 1))
                                                                If AccentIsPossible(Code) = True Then
                                                                    text = text + "&#x301;"
                                                                End If
                                                            End If
                                                        End If
                                                    End If
                                                End If
                                                
                                                text = text + "</u>"
                                            Else
                                                If words.Item(j).Characters(i).Font.Hidden = 0 And words.Item(j).Characters(i).Font.Color <> wdColorWhite Then
                                                    Code = AscW(Mid(words.Item(j).Characters(i).text, 1, 1))
                                                    text = text + Mid(words.Item(j).Characters(i).text, 1, 1)
                                                    If Not noAccent And words.Item(j).Characters(i).Font.name = "Times Roman Cyr Acsent" Then
                                                        If AccentIsPossible(Code) = True Then
                                                            text = text + "&#x301;"
                                                        End If
                                                    Else
                                                        If Not noAccent And Len(words.Item(j).Characters(i).text) > 1 Then
                                                            '0x301
                                                            Code = AscW(Mid(words.Item(j).Characters(i).text, 2, 1))
                                                            If Code = &H301 Then
                                                                Code = AscW(Mid(words.Item(j).Characters(i).text, 1, 1))
                                                                If AccentIsPossible(Code) = True Then
                                                                    text = text + "&#x301;"
                                                                End If
                                                            End If
                                                        End If
                                                    End If
                                                End If
                                            End If
                                        End If
                                    End If
                                End If
                            End If
                        Else
                            If words.Item(j).Characters(i).Font.Hidden = 0 And words.Item(j).Characters(i).Font.Color <> wdColorWhite Then
                                text = text + words.Item(j).Characters(i).text
                            End If
                        End If
                    Next i
     '           Else
     '               text = text + para.Range.Words.item(j).text
     '           End If
                length1 = Len(text)
                text = Trim$(text)
                length2 = Len(text)
                If words.Item(j).Characters.count >= 1 And words.Item(j).Characters.Item(1) <> "§" Then
                    If words.Item(j).Italic Then
                        text = text + "</i>"
                    End If
                    If words.Item(j).Bold Then
                        text = text + "</b>"
                    End If
                End If
                If (length1 > length2) Then
                    text = text & " "
                End If
        '    End If
        End If
    Next j
    
    ConvertText = DoReplacements(text)
    
End Function
Function IsDigit(Num As String) As Boolean
    digits = "0123456789"
    If Len(Num) = 0 Then
        IsDigit = False
    Else
        IsDigit = True
        For i = 1 To Len(Num)
            If InStr(digits, Mid(Num, i, 1)) = 0 Then
                IsDigit = False
                Exit For
            End If
        Next i
    End If
End Function
Function CheckText(ByRef para As Paragraph) As String
    Dim ruleText As String
    Dim cell As cell
    Dim table As table
    Dim cellpara As Paragraph
    Dim tmp As cell
    ruleText = ""
    Dim rowspan As Integer
    Dim colspan As Integer
    If para.Range.Tables.count <> 0 Then
        ruleText = "<table class=" & Chr(34) & "table table-bordered" & Chr(34) & ">"
        idx = 1
        Set table = para.Range.Tables(1)
        w = 100 / table.Columns.count
        For i = 1 To table.Columns.count
            ruleText = ruleText & "<col width=" & Chr(34) & Trim$(str$(w)) & Chr(34) & " >"
        Next i
        ruleText = ruleText & "<tr>"
        Set cell = table.cell(1, 1)
        Do While Not cell Is Nothing
            
            If cell.RowIndex <> idx Then
                ruleText = ruleText & "</tr>"
                idx = cell.RowIndex
            End If
        
            rowspan = 1
            Set tmp = cell.Next
            Do While Not tmp Is Nothing
                If cell.ColumnIndex = tmp.ColumnIndex Then
                    rowspan = tmp.RowIndex - cell.RowIndex
                    Set tmp = Nothing
                Else
                    Set tmp = tmp.Next
                End If
            Loop
            colspan = 1
            Set tmp = cell.Next
            Do While Not tmp Is Nothing
                If cell.RowIndex = tmp.RowIndex Then
                    colspan = tmp.ColumnIndex - cell.ColumnIndex
                    Set tmp = Nothing
                Else
                    If cell.RowIndex <> tmp.RowIndex Then
                        colspan = table.Columns.count - cell.ColumnIndex + 1
                        Set tmp = Nothing
                    Else
                        Set tmp = tmp.Next
                    End If
                End If
            Loop
            If rowspan > 1 And colspan > 1 Then
                ruleText = ruleText & "<td rowspan=" & Chr(34) & Trim$(str$(rowspan)) & Chr(34)
                ruleText = ruleText & " colspan=" & Chr(34) & Trim$(str$(colspan)) & Chr(34) & " >"
            Else
                If rowspan > 1 Then
                    ruleText = ruleText & "<td rowspan=" & Chr(34) & Trim$(str$(rowspan)) & Chr(34) & ">"
                Else
                    If colspan > 1 Then
                        ruleText = ruleText & "<td rowspan=" & Chr(34) & "1" & Chr(34) & " colspan=" & Chr(34) & Trim$(str$(colspan)) & Chr(34) & " >"
                    Else
                        ruleText = ruleText & "<td rowspan=" & Chr(34) & "1" & Chr(34) & " colspan=" & Chr(34) & "1" & Chr(34) & " >"
                    End If
                End If
            End If
            Set cellpara = cell.Range.Paragraphs.First
            cp = cell.Range.Paragraphs.count
            For i = 1 To cp
                ruleText = ruleText & "<p>" & ConvertText(cellpara.Range.words, 1, 0) & " </p>"
                Set cellpara = cellpara.Next
            Next i
            ruleText = ruleText & "</td>"
            
            Set cell = cell.Next
            
        Loop
        ruleText = ruleText & "</tr></table>"
        Set para = table.Range.Paragraphs.Last
        
    Else
        ruleText = ConvertText(para.Range.words, 1, para.Range.words.count)
    End If
    CheckText = ruleText
End Function
Function AccentIsPossible(ch As Integer) As Boolean
    If ch = 1072 Then      'а
        AccentIsPossible = True
        Exit Function
    End If
    If ch = 1077 Then      'е
        AccentIsPossible = True
        Exit Function
    End If
    If ch = 1080 Then 'и
        AccentIsPossible = True
        Exit Function
    End If
    If ch = 1086 Then 'о
        AccentIsPossible = True
        Exit Function
    End If
    If ch = 1091 Then 'у
        AccentIsPossible = True
        Exit Function
    End If
    If ch = 1099 Then 'ы
        AccentIsPossible = True
        Exit Function
    End If
    If ch = 1101 Then 'э
        AccentIsPossible = True
        Exit Function
    End If
    If ch = 1102 Then 'ю
        AccentIsPossible = True
        Exit Function
    End If
    If ch = 1103 Then 'я
        AccentIsPossible = True
        Exit Function
    End If
    AccentIsPossible = False

End Function


