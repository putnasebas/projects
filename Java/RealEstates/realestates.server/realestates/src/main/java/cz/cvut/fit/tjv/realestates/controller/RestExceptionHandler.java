package cz.cvut.fit.tjv.realestates.controller;

import jakarta.persistence.EntityExistsException;
import jakarta.persistence.EntityNotFoundException;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.ControllerAdvice;
import org.springframework.web.bind.annotation.ExceptionHandler;
import org.springframework.web.context.request.WebRequest;
import org.springframework.web.servlet.mvc.method.annotation.ResponseEntityExceptionHandler;

@ControllerAdvice
public class RestExceptionHandler extends ResponseEntityExceptionHandler {
    @ExceptionHandler(value = IllegalArgumentException.class)
    public ResponseEntity<Object> handleIllegalArgumentException(RuntimeException ex, WebRequest request) {
        String msg = ex.getMessage();
        return handleExceptionInternal(ex, msg, new HttpHeaders(), HttpStatus.BAD_REQUEST, request);
    }

    @ExceptionHandler(value = EntityExistsException.class)
    public ResponseEntity<Object> handleEntityExistsException(RuntimeException ex, WebRequest request) {
        String msg = ex.getMessage();
        return handleExceptionInternal(ex, msg, new HttpHeaders(), HttpStatus.UNPROCESSABLE_ENTITY, request);
    }

    @ExceptionHandler(value = EntityNotFoundException.class)
    public ResponseEntity<Object> handleEntityNotFoundException(RuntimeException ex, WebRequest request) {
        String msg = ex.getMessage();
        return handleExceptionInternal(ex, msg, new HttpHeaders(), HttpStatus.NOT_FOUND, request);
    }

}
