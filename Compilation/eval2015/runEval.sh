#!/bin/bash

# Script d'évaluation automatique des compilateurs - analyse syntaxique
# Compile le programme source et compare avec la référence

MYCOMPILO="../compilateur.out"
RENAMEXML="../rename_xml.perl"
MYOPTIONS=""
XMLDIFF="./compare_arbres_xml"
#VERBOSE="v"
VERBOSE=""

mkdir -p output
make

echo -e "Votre compilateur : ${MYCOMPILO}"
if [ ! -f  ${MYCOMPILO} ]; then
    echo -e "\033[31mCompilateur introuvable"
    echo -e "Modifiez la variable MYCOMPILO avant de lancer l'éval\033[0m"
    exit 1
fi

function test_fichier_ok() {
    input=$1
    echo -e "\n\033[4m ---- Test $input ----\033[0m"
    ${MYCOMPILO} ${MYOPTIONS} input/$input.l | ${RENAMEXML} > output/$input.synt.xml
    if [ $? != 0 ]; then 
        echo -e "\033[31mTEST ÉCHOUÉ\033[0m"
        echo -e "Le programme $input.l n'a pas été compilé correctement"	
        exit 1
    else
        echo -e "\033[32mRECO OK\033[0m"
    fi
    ${XMLDIFF} output/$input.synt.xml ref-synt/$input.synt.xml ${VERBOSE}
    if [ $? != 0 ]; then 
        echo -e "\033[31mTEST ÉCHOUÉ\033[0m"
        echo -e "Pour connaître les différences, exécutez :"
        echo -e "  diff output/$input.synt.xml ref-synt/$input.synt.xml"
        echo -e "  ${XMLDIFF} output/$input.synt.xml ref-synt/$input.synt.xml v"
        exit 1
    else
        echo -e "\033[32mARBRE OK\033[0m"
    fi
}

function test_fichier_fail() {
	input=$1
    echo -e "\n\033[4m ---- Test $input ----\033[0m"
    ${MYCOMPILO} ${MYOPTIONS} input/$input.l > output/$input.synt.xml
    if [ $? = 0 ]; then 
    echo -e "\033[31mTEST ÉCHOUÉ\033[0m"
        echo -e "Le programme $input.l a été accepté alors qu'il aurait dû être rejeté"
        exit 1
    else
        echo -e "\033[32mTEST OK\033[0m"
    fi
}

echo -e "\033[1m\n>> 1) Tests connus\033[0m"

test_fichier_ok affect
test_fichier_ok boucle
test_fichier_ok expression
test_fichier_ok max
test_fichier_ok tri

echo -e "\033[1m\n>> 1) Tests nouveaux OK\033[0m"

test_fichier_ok eval1
test_fichier_ok eval2
test_fichier_ok eval3
test_fichier_ok eval4
test_fichier_ok eval5
test_fichier_ok eval6
test_fichier_ok eval7
test_fichier_ok eval8

echo -e "\033[1m\n>> 1) Tests nouveaux FAIL\033[0m"

test_fichier_fail eval1err
test_fichier_fail eval2err
test_fichier_fail eval3err
test_fichier_fail eval4err
test_fichier_fail eval5err

echo -e "\033[1m\n>> 1) Tests nouvelle fonctionnalité OK\033[0m"

test_fichier_ok test1
test_fichier_ok test2
test_fichier_ok test3

echo -e "\033[1m\n>> 1) Tests nouvelle fonctionnalité FAIL\033[0m"

test_fichier_fail test1err

