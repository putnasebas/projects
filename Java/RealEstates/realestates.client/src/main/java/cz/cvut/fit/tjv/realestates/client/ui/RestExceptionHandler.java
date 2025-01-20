package cz.cvut.fit.tjv.realestates.client.ui;

import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.ControllerAdvice;
import org.springframework.web.bind.annotation.ExceptionHandler;
import org.springframework.web.client.HttpClientErrorException;
import org.springframework.web.client.HttpServerErrorException;

@ControllerAdvice
public class RestExceptionHandler {

    @ExceptionHandler(HttpClientErrorException.class)
    public String handleClientError(Model model, HttpClientErrorException ex) {
        String errorMessage = "Client error: " + ex.getStatusCode() + " - " + ex.getResponseBodyAsString();
        model.addAttribute("errorMessage", errorMessage);
        return "ErrorPage";
    }

    @ExceptionHandler(HttpServerErrorException.class)
    public String handleServerError(Model model, HttpServerErrorException ex) {
        String errorMessage = "Server error: " + ex.getStatusCode() + " - " + ex.getResponseBodyAsString();
        model.addAttribute("errorMessage", errorMessage);
        return "ErrorPage";
    }

    @ExceptionHandler(Exception.class)
    public String handleGenericError(Model model, Exception ex) {
        model.addAttribute("errorMessage", "An unexpected error occurred: " + ex.getMessage());
        return "ErrorPage";
    }
}
