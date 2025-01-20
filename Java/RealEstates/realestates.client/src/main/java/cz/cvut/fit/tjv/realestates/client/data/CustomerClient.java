package cz.cvut.fit.tjv.realestates.client.data;

import cz.cvut.fit.tjv.realestates.client.model.CustomerDTO;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.http.HttpStatus;
import org.springframework.http.HttpStatusCode;
import org.springframework.http.MediaType;
import org.springframework.stereotype.Component;
import org.springframework.web.client.HttpClientErrorException;
import org.springframework.web.reactive.function.client.WebClient;
import reactor.core.publisher.Flux;
import reactor.core.publisher.Mono;


@Component
public class CustomerClient {
    private static final String CUSTOMER_URI = "/{id}";
    private final WebClient customerWebClient;

    public CustomerClient(@Value("${backend.url}") String backendUrl) {
        this.customerWebClient = WebClient.create(backendUrl + "/rest/api/customer");
    }

    public Mono<CustomerDTO> getCustomerById(String email) {
        // Mono = zero/one item is expected
        return customerWebClient.get()
                .uri(CUSTOMER_URI, email)
                .retrieve()
                .onStatus(HttpStatusCode::is4xxClientError, clientResponse ->
                        clientResponse.bodyToMono(String.class)
                                .flatMap(errorBody -> Mono.error(new HttpClientErrorException(HttpStatus.BAD_REQUEST, errorBody))))
                .bodyToMono(CustomerDTO.class);
    }

    public Flux<CustomerDTO> getAllCustomers() {
        // Flux = zero -> many items expected
        return customerWebClient.get()
                .retrieve()
                .bodyToFlux(CustomerDTO.class);
    }

    public Mono<CustomerDTO> createCustomer(CustomerDTO customerDTO) {
        return customerWebClient.post()
                .contentType(MediaType.APPLICATION_JSON)
                .bodyValue(customerDTO)
                .retrieve()
                .bodyToMono(CustomerDTO.class);
    }

    public Mono<CustomerDTO> updateCustomer(CustomerDTO customerDTO, String email) {
        return customerWebClient.put()
                .uri(CUSTOMER_URI, email)
                .contentType(MediaType.APPLICATION_JSON)
                .bodyValue(customerDTO)
                .retrieve()
                .bodyToMono(CustomerDTO.class);
    }

    public Mono<Void> deleteCustomerById(String email) {
        return customerWebClient.delete()
                .uri(CUSTOMER_URI, email)
                .retrieve()
                .bodyToMono(Void.class);
    }

    public Mono<Void> deleteCustomersByAge() {
        return customerWebClient.delete()
                .uri("/date")
                .retrieve()
                .bodyToMono(Void.class);
    }
}
