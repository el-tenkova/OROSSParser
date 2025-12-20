Attribute VB_Name = "Cnv2HTML"
Dim footNote As Integer

Function AccentIsPossible(ch As Integer) As Boolean
    If ch = 1072 Then      'à
        AccentIsPossible = True
        Exit Function
    End If
    If ch = 1077 Then      'å
        AccentIsPossible = True
        Exit Function
    End If
    If ch = 1080 Then 'è
        AccentIsPossible = True
        Exit Function
    End If
    If ch = 1086 Then 'î
        AccentIsPossible = True
        Exit Function
    End If
    If ch = 1091 Then 'ó
        AccentIsPossible = True
        Exit Function
    End If
    If ch = 1099 Then 'û
        AccentIsPossible = True
        Exit Function
    End If
    If ch = 1101 Then 'ý
        AccentIsPossible = True
        Exit Function
    End If
    If ch = 1102 Then 'þ
        AccentIsPossible = True
        Exit Function
    End If
    If ch = 1103 Then 'ÿ
        AccentIsPossible = True
        Exit Function
    End If
    AccentIsPossible = False

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
        End If
    Next j
    
    ConvertText = DoReplacements(text)
    
End Function

Sub ConvertDocument()
   
    Dim theDoc As Document
    Set theDoc = Application.ActiveDocument
    
    actName = theDoc.Path & "\" & theDoc.name
    
    Set para = theDoc.Paragraphs.Item(1)
    cp = theDoc.Paragraphs.count
    
    footNote = 1
    
    Dim fst As Object
    Set fst = CreateObject("ADODB.Stream")
    fst.Type = 2
    fst.Charset = "utf-8"
    fst.Open
    
    For i = 1 To cp
        If para.Range.Characters.count > 1 Then
            Dim text2Cnv As String
            text2Cnv = ConvertText(para.Range.words, 1, 0)
            fst.WriteText ("<div>" & text2Cnv & "</div>" & vbCrLf)
        End If
        Set para = para.Next()
        If para Is Nothing Then
            Exit For
        End If
    Next i
    
    Dim note As footNote
    Dim cn As Long
    Dim idx As Long
    Dim fNotes As New Collection
    
    ' footnotes
    cn = theDoc.Range.Footnotes.count
    idx = 1
    If cn > 0 Then
        fst.WriteText ("<hr>" & vbCrLf)
        For i = 1 To cn
            Set note = theDoc.Range.Footnotes.Item(i)
            Dim text As String
            text = ""
            text = text & "<sup><a href=" & Chr(34) & "#ft" & Trim$(str$(idx)) & Chr(34)
            text = text & " id=" & Chr(34) & "foot" & Trim$(str$(idx)) & Chr(34) & " >"
            text = text & "[" & Trim$(str$(idx)) & "]</a></sup>"
            
            text = text & ConvertText(note.Range.words, 1, 0, False, True)
            fst.WriteText ("<div>" & text & "</div>" & vbCrLf)
            idx = idx + 1
        Next i
    End If
    
    fst.SaveToFile actName & ".html", 2
    fst.Close
    
End Sub

Re: Академос: добавление новых статей
