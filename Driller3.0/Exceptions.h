#pragma once
#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

class Exception {};
class ExceptionIndexOutOfRange : Exception {};
class ExceptionMemoryNotAvailable : Exception {};
class ExceptionLinkedListAccess : Exception {};
class ExceptionEnumerationBeyondEnd : Exception {};
class ExceptionHashTableAccess : Exception {};

#endif